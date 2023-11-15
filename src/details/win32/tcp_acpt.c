#include "tcp_acpt.h"
#include "tcp.h"

#include "io_res.h"

obj_trait __tcp_acpt_trait					= {
	.init		   = &__tcp_acpt_init		  ,
	.init_as_clone = &__tcp_acpt_init_as_clone,
	.init_as_ref   =						 0,
	.deinit		   = &__tcp_acpt_deinit		  ,
	.name		   =						 0,
	.size		   = &__tcp_acpt_size
};

bool_t 
	__tcp_acpt_init
		(__tcp_acpt* par_acpt, u32_t par_count, va_list par)         {
			if (env == INVALID_SOCKET)
				if (!__env_init()) return false_t;

			par_acpt->io_sched = ref	   (va_arg(par, __io_sched*));
			par_acpt->tcp	   = WSASocket (
				AF_INET			   ,
				SOCK_STREAM		   ,
				IPPROTO_TCP		   ,
				0				   ,
				0				   ,
				WSA_FLAG_OVERLAPPED
			);

			if (par_acpt->tcp == INVALID_SOCKET) {
				del(par_acpt->io_sched);
				return false_t;
			}

			return true_t;
}

bool_t 
	__tcp_acpt_init_as_clone
		(__tcp_acpt* par, __tcp_acpt* par_clone) {
			return false_t;
}

void   
	__tcp_acpt_deinit
		(__tcp_acpt* par)			  {
			closesocket(par->tcp)     ;
			del		   (par->io_sched);
}

u64_t
	__tcp_acpt_size() {
		return sizeof(__tcp_acpt);
}

bool_t 
	__tcp_acpt_conn
		(__tcp_acpt* par, const char* par_v4, u16_t par_port) {
			par->v4.sin_addr.s_addr = inet_addr(par_v4)  ;
			par->v4.sin_port		= htons	   (par_port);
			par->v4.sin_family	    = AF_INET			 ;

			if (bind(par->tcp, &par->v4, sizeof(struct sockaddr_in)))
				return false_t;
			if (listen(par->tcp, -1))
				return false_t;

			return true_t;
}

void
	__tcp_acpt_close
		(__tcp_acpt* par)			  {
			closesocket(par->tcp)	  ;
			del		   (par->io_sched);
}

task* 
	__tcp_acpt_run
		(__tcp_acpt* par)											   {
			__tcp *ret_tcp = make (&__tcp_trait) from(1, par->io_sched);
			if   (!ret_tcp)
				return false_t;

			__io_res* ret = make(&__io_res_trait) from(0); 
			if (!ret)		  {
				del(ret_tcp)  ;
				return false_t;
			}

			u32_t  ret_size = 0			  ;
			bool_t ret_res  = AcceptEx    (
				par    ->tcp			  ,
				ret_tcp->tcp			  ,
				&ret_tcp->v4			  ,
				0						  ,
				sizeof(struct sockaddr_in),
				sizeof(struct sockaddr_in),
				&ret_size				  ,
				&ret->hnd
			);
			
			ret->ret = ret_tcp;
			if(!ret_res)	{
				del(ret_tcp);
				del(ret)    ;

				return 0;
			}

			return ret->task;
}