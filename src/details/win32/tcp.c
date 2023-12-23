#include "tcp.h"

#include "io_sched.h"
#include "io_task.h"

#include "curr.h"
#include "task.h"
#include "thd.h"

obj_trait tcp_trait	    = {
	.on_new	  = &tcp_new  ,
	.on_clone = &tcp_clone,
	.on_ref   =		     0,
	.on_del	  = &tcp_del  ,
	.size	  = sizeof(tcp)
};

obj_trait* tcp_t = &tcp_trait;

bool_t 
	tcp_new
		(tcp* par_tcp, u32_t par_count, va_list par) {
			par_tcp->io_sched     = (par_count == 1) ? va_arg(par, io_sched*) : ref(&curr_thd->io_sched);
			if (!par_tcp->io_sched)					       return false_t;
			if (trait_of(par_tcp->io_sched) != io_sched_t) return false_t;
			if (!make_at(&par_tcp->v4, v4_t) from (0))     return false_t;

			par_tcp->tcp	      = INVALID_SOCKET		  ;
			par_tcp->tcp_io_sched = INVALID_HANDLE_VALUE  ;
			par_tcp->io_sched	  = ref(par_tcp->io_sched);

			return true_t;
}

bool_t 
	tcp_clone
		(tcp* par, tcp* par_clone) {
			return false_t;
}

void
	tcp_del
		(tcp* par)				      {
			del		   (par->io_sched);
			closesocket(par->tcp)	  ;
}

void*
	tcp_conn_do
		(tcp* par)												{
			io_task* res_task = io_sched_dispatch(par->io_sched); res_task->state = io_task_exec;
			bool_t   res      = ConnectEx (
				par->tcp				  ,
				&par->v4.v4			      ,
				sizeof(struct sockaddr_in),
				0						  ,
				0						  ,
				0						  ,
				&res_task->io_task
			);

			if (!res && WSAGetLastError() != ERROR_IO_PENDING) {
				list_push_back(&par->io_sched->none, res_task);
				res_task->state = io_task_free				  ;

				return 0;
			}

			io_task_wait(res_task);
			return par;
}

struct task*
	tcp_conn
		(tcp* par, v4* par_v4)		        {
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
			par->v4.v4.sin_family	   = AF_INET;
			par->v4.v4.sin_addr.s_addr = 0	    ;
			par->v4.v4.sin_port		   = 0	    ;

			if (bind(par->tcp, &par->v4.v4, sizeof(struct sockaddr_in))) goto conn_failed;
			if (!clone_at(&par->v4, par_v4))						     goto conn_failed;

			task  *ret = sched_dispatch(curr_sched, tcp_conn_do, par); if (!ret) goto conn_failed;
			return ret;

	conn_failed:
			closesocket(par->tcp)			  ;
			par->tcp		  = INVALID_SOCKET;
			par->tcp_io_sched = 0			  ;

			return 0;
}

void 
	tcp_close
		(tcp* par)				 {
			closesocket(par->tcp); 
}

struct io_task*
	tcp_send
		(tcp* par, u8_t* par_buf, u64_t par_len)				  {
			io_task* ret	 = io_sched_dispatch(par->io_sched)   ; if (!ret) return false_t;
			WSABUF   ret_buf = { .buf = par_buf, .len = par_len  };
			i32_t	 res     = WSASend (
				par->tcp	 ,
				&ret_buf	 ,
				1			 ,
				0			 ,
				0			 ,
				&ret->io_task,
				0
			);

			ret->state = io_task_exec;
			if (res && WSAGetLastError() != ERROR_IO_PENDING) {
				ret->state = io_task_free			     ;
				list_push_back(&par->io_sched->none, ret);

				return 0;
			}

			return ret;
}

struct io_task*
	tcp_recv
		(tcp* par, u8_t* par_buf, u64_t par_len)				   {
			u32_t	 ret_flags = 0								   ;
			WSABUF	 ret_buf   = { .buf = par_buf, .len = par_len };
			io_task *ret       = io_sched_dispatch(par->io_sched)  ; if (!ret) return false_t;
			i32_t    res	   = WSARecv (
				par->tcp	 ,
				&ret_buf	 ,
				1			 ,
				0			 ,
				&ret_flags   ,
				&ret->io_task, 
				0
			);

			ret->state = io_task_exec;
			if (res && WSAGetLastError() != ERROR_IO_PENDING) {
				ret->state = io_task_free				 ;
				list_push_back(&par->io_sched->none, ret);

				return 0;
			}

			return ret;
}