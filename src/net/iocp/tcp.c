#include "tcp.h"

#include "v4.h"
#include "v6.h"

obj_trait tcp_trait	= make_trait (
	tcp_new	   ,
	tcp_clone  ,
	null_t	   ,
	tcp_del	   ,
	sizeof(tcp),
	null_t
);

obj_trait* tcp_t = &tcp_trait;

bool_t 
	tcp_new
		(tcp* par_tcp, u32_t par_count, va_list par)							  {
			io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
			if (trait_of(sched) != io_sched_t) return false_t;
			par_tcp->tcp_io = 0				;
			par_tcp->tcp    = INVALID_SOCKET;
			par_tcp->sched  = ref(sched)    ;
			par_tcp->flag   = 0		        ;
			return true_t;
}

bool_t 
	tcp_clone
		(tcp* par, tcp* par_clone) {
			return false_t;
}

void
	tcp_del
		(tcp* par)				   {
			closesocket(par->tcp)  ;
			del		   (par->sched);
			
}

bool_t 
	tcp_open
		(tcp* par, obj_trait* par_af)							   {
			if (trait_of(par) != tcp_t) return false_t; int af = -1;
			if (par_af == v4_t) af = AF_INET  ;
			if (par_af == v6_t) af = AF_INET6 ;
			if (af == -1)		return false_t;

			if (par->tcp != INVALID_SOCKET) return false_t;
			par->tcp = WSASocket						  (
				af				  ,
				SOCK_STREAM		  ,
				IPPROTO_TCP		  ,
				0				  ,
				0				  ,
				WSA_FLAG_OVERLAPPED
			);

			if (par->tcp == INVALID_SOCKET) return false_t;
			par->tcp_io = CreateIoCompletionPort		  (
				par->tcp	   ,
				par->sched->hnd,
				par->sched	   , 
				0
			);

			if (!par->tcp_io)		     {
				closesocket(par->tcp)    ;
				par->tcp = INVALID_SOCKET;
				return false_t;
			}
			return true_t;
}

fut*
	tcp_conn
		(tcp* par, end* par_end)							  {
			if (trait_of(par_end) != end_t)		 return null_t;
			if (trait_of(par)     != tcp_t)	     return null_t;
			if (!tcp_open(par, end_af(par_end))) return null_t;
			SOCKADDR_IN6 end;
			
			mem_set(&end, 0x00, sizeof(end));
			end.sin6_family = par_end->af;
			if (bind(par->tcp, &end, par_end->len)) {
				tcp_close(par);
				return 0;
			}

			io_res *ret = make(io_res) from (1, par->sched); if (trait_of(ret) != io_res_t) return 0;
			bool_t  res = ConnectEx						   (
				par->tcp	 ,
				&par_end->all,
				par_end->len ,
				0			 ,
				0			 ,
				0			 ,
				&ret->res
			);

			fut*   fut = io_res_fut(ret);
			del   (fut);
			return fut;
}

void 
	tcp_close
		(tcp* par)							  {
			if (trait_of(par) != tcp_t) return;
			closesocket(par->tcp)		;
			par->tcp_io = 0				;
			par->tcp    = INVALID_SOCKET;
}

fut*
	tcp_send
		(tcp* par, u8_t* par_buf, u64_t par_len)	 {
			if (trait_of(par) != tcp_t) return null_t;
			if (!par_len)				return null_t;
			if (!par_buf)				return null_t;
			WSABUF  buf							   = { 
				.buf = par_buf,
				.len = par_len 
			};
			
			io_res* ret = make (io_res) from (1, par->sched); if (trait_of(ret) != io_res_t) return 0;
			i32_t   res = WSASend						    (
				par->tcp ,
				&buf	 ,
				1		 ,
				0		 ,
				par->flag,
				&ret->res,
				0
			);

			fut* fut = io_res_fut(ret);
			del (ret);
			if  (res && WSAGetLastError() != ERROR_IO_PENDING) {
				 ret->stat = fut_err		  ;
				 ret->ret  = WSAGetLastError();
				 return fut;
			}

			return fut;
}

fut*
	tcp_recv
		(tcp* par, u8_t* par_buf, u64_t par_len)     {
			if (trait_of(par) != tcp_t) return null_t;
			if (!par_len)				return null_t;
			if (!par_buf)				return null_t;
			WSABUF buf							   = { 
				.buf = par_buf, 
				.len = par_len 
			};

			io_res *ret = make (io_res) from (1, par->sched); if (trait_of(ret) != io_res_t) return 0;
			i32_t   res = WSARecv							(
				par->tcp  ,
				&buf	  ,
				1		  ,
				0		  ,
				&par->flag,
				&ret->res ,
				0
			);

			fut* fut = io_res_fut(ret);
			del (ret);
			if  (res && WSAGetLastError() != ERROR_IO_PENDING) {
			 	 ret->stat = fut_err		  ;
				 ret->ret  = WSAGetLastError();
				 return fut;
			}

			return fut;
}