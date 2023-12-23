#include "tcp_acpt.h"
#include "tcp.h"

#include "io_sched.h"
#include "io_task.h"

#include "thd.h"
#include "curr.h"

obj_trait tcp_acpt_trait     = {
	.on_new	  = &tcp_acpt_new  ,
	.on_clone = &tcp_acpt_clone,
	.on_ref   =				  0,
	.on_del	  = &tcp_acpt_del  ,
	.size	  = sizeof(tcp_acpt)
};

obj_trait* tcp_acpt_t = &tcp_acpt_trait;

bool_t 
	tcp_acpt_new
		(tcp_acpt* par_acpt, u32_t par_count, va_list par)							   		    {
			par_acpt->io_sched = (par_count == 1) ? va_arg(par, io_sched*) : &curr_thd->io_sched;
			if (!par_acpt->io_sched)						return false_t;
			if (trait_of(par_acpt->io_sched) != io_sched_t) return false_t;

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
	tcp_acpt_clone
		(tcp_acpt* par, tcp_acpt* par_clone) {
			return false_t;
}

void   
	tcp_acpt_del
		(tcp_acpt* par)			      {
			closesocket(par->tcp)     ;
			CloseHandle(par->tcp_iocp);
			del		   (par->io_sched);
}

bool_t 
	tcp_acpt_conn
		(tcp_acpt* par, v4* par_addr)									                 {
			if (bind(par->tcp, &par_addr->v4, sizeof(struct sockaddr_in))) return false_t;
			if (listen(par->tcp, -1))									   return false_t;
			
			return true_t;
}

void
	tcp_acpt_close
		(tcp_acpt* par)			      {
			closesocket(par->tcp)	  ;
			del		   (par->io_sched);
}

void*
	tcp_acpt_run_do
		(tcp_acpt* par)									   {
			u8_t ret_addr[64]							   ;
			tcp *ret = make (tcp_t) from (1, par->io_sched);
			if (!ret) return NULL;

			ret->tcp = WSASocket   (
				AF_INET			   ,
				SOCK_STREAM		   ,
				IPPROTO_TCP		   ,
				0				   ,
				0				   ,
				WSA_FLAG_OVERLAPPED
			);

			if (ret->tcp == INVALID_SOCKET) goto do_failed;
			ret->tcp_io_sched = CreateIoCompletionPort (
				ret->tcp			   ,
				ret->io_sched->io_sched,
				ret->io_sched		   ,
				0
			);

			if (!ret->tcp_io_sched) goto do_failed;
			io_task* ret_task = io_sched_dispatch(par->io_sched); ret_task->state = io_task_exec;
			u32_t    res_size = 0			   ;
			bool_t   res	  = AcceptEx	   (
				par ->tcp				       ,
				ret ->tcp				       ,
				ret_addr					   ,
				0							   ,
				sizeof(struct sockaddr_in) + 16,
				sizeof(struct sockaddr_in) + 16,
				&res_size					   ,
				&ret_task->io_task
			);

			if (!res && WSAGetLastError() != ERROR_IO_PENDING) {
				ret_task->state = io_task_free;
				list_push_back(&par->io_sched->none, ret);
				
				goto do_failed;
			}

			io_task_wait(ret_task);
			return ret;
	do_failed:
			del (ret);
			return false_t;
}

struct task*
	tcp_acpt_run
		(tcp_acpt* par)								 {
			if (!par)						 return 0;
			if (trait_of(par) != tcp_acpt_t) return 0;

			return sched_dispatch(curr_sched, tcp_acpt_run_do, par);
}