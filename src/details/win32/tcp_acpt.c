#include "tcp_acpt.h"
#include "tcp.h"

#include "io_sched.h"
#include "io_task.h"

#include "thd.h"
#include "curr.h"

obj_trait __tcp_acpt_trait     = {
	.on_new	  = &__tcp_acpt_new	 ,
	.on_clone = &__tcp_acpt_clone,
	.on_ref   =				    0,
	.on_del	  = &__tcp_acpt_del	 ,
	.size	  = sizeof(__tcp_acpt)
};

bool_t 
	__tcp_acpt_new
		(__tcp_acpt* par_acpt, u32_t par_count, va_list par)  {
			par_acpt->io_sched = (par_count == 1) ? ref(va_arg(par, __io_sched*)) : ref(&curr_thd->io_sched);
			if (!par_acpt->io_sched)
				return false_t;

			par_acpt->tcp = WSASocket (AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
			if (par_acpt->tcp == INVALID_SOCKET) {
				del(par_acpt->io_sched);
				return false_t;
			}

			par_acpt->tcp_iocp = CreateIoCompletionPort (
				par_acpt->tcp				,
				par_acpt->io_sched->io_sched, 
				par_acpt->io_sched			,
				0
			);

			if(!par_acpt->tcp_iocp)			   {
				closesocket(par_acpt->tcp)	   ;
				del		   (par_acpt->io_sched);
				return false_t;
			}

			return true_t;
}

bool_t 
	__tcp_acpt_clone
		(__tcp_acpt* par, __tcp_acpt* par_clone) {
			return false_t;
}

void   
	__tcp_acpt_del
		(__tcp_acpt* par)			  {
			closesocket(par->tcp)     ;
			CloseHandle(par->tcp_iocp);
			del		   (par->io_sched);
}

bool_t 
	__tcp_acpt_conn
		(__tcp_acpt* par, __v4* par_addr)	  {
			if (bind(par->tcp, &par_addr->host, sizeof(struct sockaddr_in))) return false_t;
			if (listen(par->tcp, -1))										 return false_t;
			
			return true_t;
}

void
	__tcp_acpt_close
		(__tcp_acpt* par)			  {
			closesocket(par->tcp)	  ;
			del		   (par->io_sched);
}

struct __io_task*
	__tcp_acpt_run
		(__tcp_acpt* par)											   {
			__tcp *ret_tcp = make (&__tcp_trait) from(1, par->io_sched);

			if (!ret_tcp) return false_t;
			ret_tcp->tcp = WSASocket (
				AF_INET			   ,
				SOCK_STREAM		   ,
				IPPROTO_TCP		   ,
				0				   ,
				0				   ,
				WSA_FLAG_OVERLAPPED
			);

			if (ret_tcp->tcp == INVALID_SOCKET) goto run_failed;
			ret_tcp->tcp_io_sched = CreateIoCompletionPort (
				ret_tcp->tcp			   ,
				ret_tcp->io_sched->io_sched, 
				ret_tcp->io_sched		   ,
				0
			);

			if (!ret_tcp->tcp_io_sched) goto run_failed;
			__io_task* ret		= __io_sched_dispatch(par->io_sched); if (!ret) goto run_failed;
			u32_t      res_size = 0			   ;
			bool_t     res		= AcceptEx	   (
				par     ->tcp				   ,
				ret_tcp ->tcp				   ,
				&ret_tcp->v4.host			   ,
				0							   ,
				sizeof(struct sockaddr_in) + 16,
				sizeof(struct sockaddr_in) + 16,
				&res_size					   ,
				&ret->io_task
			);
			
			ret->ret   = ret_tcp			 ;
			ret->state = __io_task_state_pend;
			if(!res && WSAGetLastError() != ERROR_IO_PENDING) {
				ret->state = __io_task_state_none			  ;
				list_push_back(&par->io_sched->io_task, ret)  ;
				del(ret_tcp);
				return     0;
			}

			return ret;
	run_failed:
			del(ret_tcp);
			return     0;
}