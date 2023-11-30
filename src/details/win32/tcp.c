#include "tcp.h"

#include "io_sched.h"
#include "io_task.h"

#include "curr.h"
#include "thd.h"

#include <stdio.h>

obj_trait __tcp_trait	  = {
	.on_new	  = &__tcp_new  ,
	.on_clone = &__tcp_clone,
	.on_ref   =			   0,
	.on_del	  = &__tcp_del  ,
	.size	  = sizeof(__tcp)
};

bool_t 
	__tcp_new
		(__tcp* par_tcp, u32_t par_count, va_list par)  {
			par_tcp->io_sched     = (par_count == 1) ? ref(va_arg(par, __io_sched*)) : ref(&curr_thd->io_sched);
			par_tcp->tcp	      = INVALID_SOCKET		;
			par_tcp->tcp_io_sched = INVALID_HANDLE_VALUE;

			return make_at(&par_tcp->v4, &__v4_trait) from (0);
}

bool_t 
	__tcp_clone
		(__tcp* par, __tcp* par_clone) {
			return false_t;
}

void
	__tcp_del
		(__tcp* par)				  {
			del		   (par->io_sched);
			closesocket(par->tcp)	  ;
}

struct __io_task*
	__tcp_conn
		(__tcp* par, __v4* par_v4)		    {
			if (par->tcp == INVALID_SOCKET) {
				par->tcp = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
				if (par->tcp == INVALID_SOCKET)
					return 0;
			}

			par->tcp_io_sched = CreateIoCompletionPort (
				par->tcp			   ,
				par->io_sched->io_sched, 
				par->io_sched		   ,
				0
			);

			if (!par->tcp_io_sched) goto conn_failed;
			par->v4.host.sin_family		 = AF_INET;
			par->v4.host.sin_addr.s_addr = 0	  ;
			par->v4.host.sin_port		 = 0	  ;

			if (bind(par->tcp, &par->v4.host, sizeof(struct sockaddr_in))) goto conn_failed;
			if (!clone_at(&par->v4, par_v4))							   goto conn_failed;
			__io_task* ret = __io_sched_dispatch(par->io_sched); if (!ret) goto conn_failed;
			bool_t     res = ConnectEx	  (
				par->tcp				  ,
				&par->v4.host			  ,
				sizeof(struct sockaddr_in),
				0						  ,
				0						  ,
				0						  ,
				&ret->io_task
			);

			ret->state = __io_task_state_pend;
			if (!res && WSAGetLastError() != ERROR_IO_PENDING)  {
				ret->state = __io_task_state_none				;
				obj_list_push_back(&par->io_sched->io_task, ret);

				goto conn_failed;
			}

			return ret;
		conn_failed:
			closesocket(par->tcp)    ;
			par->tcp = INVALID_SOCKET;

			return 0;
}

void __tcp_close(__tcp* par) { closesocket(par->tcp); }

struct __io_task*
	__tcp_send
		(__tcp* par, u8_t* par_buf, u64_t par_len)				    {
			__io_task* ret	   = __io_sched_dispatch(par->io_sched) ; if (!ret) return false_t;
			WSABUF     ret_buf = { .buf = par_buf, .len = par_len  };
			i32_t	   res     = WSASend (
				par->tcp	 ,
				&ret_buf	 ,
				1			 ,
				0			 ,
				0			 ,
				&ret->io_task,
				0
			);

			ret->state = __io_task_state_pend;
			if (res && WSAGetLastError() != ERROR_IO_PENDING)   {
				ret->state = __io_task_state_none				;
				obj_list_push_back(&par->io_sched->io_task, ret);

				return 0;
			}

			return ret;
}

struct __io_task*
	__tcp_recv
		(__tcp* par, u8_t* par_buf, u64_t par_len) {
			u32_t	   ret_flags = 0;
			WSABUF	   ret_buf   = { .buf = par_buf, .len = par_len };
			__io_task *ret       = __io_sched_dispatch(par->io_sched); if (!ret) return false_t;
			i32_t      res		 = WSARecv (
				par->tcp	 ,
				&ret_buf	 ,
				1			 ,
				0			 ,
				&ret_flags   ,
				&ret->io_task, 
				0
			);

			ret->state = __io_task_state_pend;
			if (res && WSAGetLastError() != ERROR_IO_PENDING)   {
				ret->state = __io_task_state_none				;
				obj_list_push_back(&par->io_sched->io_task, ret);

				return 0;
			}

			return ret;
}