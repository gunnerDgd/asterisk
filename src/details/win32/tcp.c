#include "tcp.h"
#include "io_res.h"

obj_trait __tcp_trait				   = {
	.init		   = &__tcp_init		 ,
	.init_as_clone = &__tcp_init_as_clone,
	.init_as_ref   =					0,
	.deinit		   = &__tcp_deinit		 ,
	.name		   =				    0,
	.size		   = &__tcp_size
};

bool_t 
	__tcp_init
		(__tcp* par_tcp, u32_t par_count, va_list par)		 {
			par_tcp->io_sched = ref(va_arg(par, __io_sched*));
			par_tcp->tcp	  = INVALID_SOCKET		;
			par_tcp->tcp_iocp = INVALID_HANDLE_VALUE;

			memset(&par_tcp->v4, 0x00, sizeof(par_tcp->v4));
			memset(&par_tcp->v6, 0x00, sizeof(par_tcp->v6));
			
			return true_t;
}

bool_t 
	__tcp_init_as_clone
		(__tcp* par, __tcp* par_clone) {
			return false_t;
}

void
	__tcp_deinit
		(__tcp* par) {
			task* ret = __tcp_close(par);
			await(ret)			;
			del  (ret)			;
			del  (par->io_sched);

			closesocket(par->tcp);
}

u64_t
	__tcp_size() {
		return sizeof(__tcp);
}

task*
	__tcp_conn
		(__tcp* par, __v4* par_v4) {
			if(par->tcp == INVALID_SOCKET) {
				par->tcp = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
				if (par->tcp == INVALID_SOCKET)
					return 0;
			}

			par->tcp_iocp = CreateIoCompletionPort (par->tcp, par->io_sched->hnd, par->io_sched, 0);
			if(!par->tcp_iocp)		 {
				closesocket(par->tcp);
				return 0;
			}

			par->v4.host.sin_family = AF_INET;
			if (bind(par->tcp, &par->v4.host, sizeof(struct sockaddr_in))) {
				par->tcp = INVALID_SOCKET;
				return 0;
			}

			__io_res* res = make(&__io_res_trait) from(1, par->io_sched);
			if (!res)
				return 0;

			par->v4.host	  = par_v4->v4;
			bool_t ret = ConnectEx		  (
				par->tcp				  ,
				&par->v4.host			  ,
				sizeof(struct sockaddr_in),
				0						  ,
				0						  ,
				0						  ,
				&res->hnd
			);

			if (!ret && WSAGetLastError() != ERROR_IO_PENDING) {
				del(res);
				return 0;
			}

			return res->task;
}

task*
	__tcp_close  
		(__tcp* par) {
			__io_res* res = make(&__io_res_trait) from (1, par->io_sched);
			bool_t    ret = DisconnectEx (
				par->tcp ,
				&res->hnd,
				0		 ,
				0
			);

			return res->task;
}

task*
	__tcp_send
		(__tcp* par, ptr par_buf, u64_t par_len) {
			if(par_len > ptr_size(par_buf))
				return 0;

			__io_res* res = make(&__io_res_trait) from(1, par->io_sched);
			if (!res)
				return false_t;

			WSABUF ret_buf			 = { 
				.buf = ptr_raw(par_buf), 
				.len = par_len 
			};

			i32_t ret = WSASend(par->tcp, &ret_buf, 1, 0, 0, &res->hnd, 0); 
			if   (ret && WSAGetLastError() != ERROR_IO_PENDING) {
				del(res);
				return 0;
			}

			return res->task;
}

task*
	__tcp_recv
		(__tcp* par, ptr par_buf, u64_t par_len) {
			if(par_len > ptr_size(par_buf))
				return 0;

			__io_res *res = make(&__io_res_trait) from(1, par->io_sched);
			if (!res)
				return false_t;

			u32_t  ret_flags =		  0;
			WSABUF ret_buf   =		   {
				.buf = ptr_raw(par_buf),
				.len = par_len 
			};

			i32_t ret = WSARecv(par->tcp, &ret_buf, 1, 0, &ret_flags, &res->hnd, 0);
			if   (ret && WSAGetLastError() != ERROR_IO_PENDING) {
				del(res);
				return 0;
			}

			return res->task;
}