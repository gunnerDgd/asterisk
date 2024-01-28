#include "tcp_acpt.h"
#include "tcp.h"

#include "io_sched.h"
#include "io_res.h"

#include "this.h"
#include "thd.h"

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
		(tcp_acpt* par_acpt, u32_t par_count, va_list par)						  {
			io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
			if (!sched)											 return false_t;
			if (trait_of(sched_t) != io_sched_t)				 return false_t;
			if (!make_at(&par_acpt->tcp, tcp_t) from (1, sched)) return false_t;
			
			par_acpt->sched = ref(sched);
			return true_t;
}

bool_t 
	tcp_acpt_clone
		(tcp_acpt* par, tcp_acpt* par_clone) {
			return false_t;
}

void   
	tcp_acpt_del
		(tcp_acpt* par)	    {
			del (&par->tcp) ;
			del (par->sched);
}

bool_t
	tcp_acpt_conn
		(tcp_acpt* par, obj* par_addr)					   {
			if (!par_addr)					 return false_t;
			if (!par)						 return false_t;
			if (trait_of(par) != tcp_acpt_t) return false_t;
			
			if (trait_of(par_addr) == v4_t)  return tcp_acpt_conn_v4(par, par_addr);
			return false_t;
}

bool_t 
	tcp_acpt_conn_v4
		(tcp_acpt* par, v4* par_addr)							{
			if (!par_addr)						  return false_t;
			if (!par)						      return false_t;
			if (trait_of(par_addr) != v4_t)		  return false_t;
			if (trait_of(par)      != tcp_acpt_t) return false_t;

			SOCKET			   tcp = par->tcp.tcp;
			struct sockaddr_in v4  = par_addr->v4;
			if (bind  (tcp, &v4, sizeof(v4))) return false_t;
			if (listen(tcp, -1))			  return false_t;
			return true_t;
}

void
	tcp_acpt_close
		(tcp_acpt* par)	   {
			del(par->sched);
			del(&par->tcp) ;
}

void*
	tcp_acpt_run_do
		(tcp_acpt* par)											  {
			tcp    *tcp	    = make (tcp_t)    from (1, par->sched);
			io_res *ret		= make (io_res_t) from (1, par->sched);
			u8_t    buf[64] = { 0x00, }						 ;
			u32_t   len     = sizeof(struct sockaddr_in) + 16;

			if (!tcp)					   return 0;
			if (!ret)					   return 0;
			if (trait_of(tcp) != tcp_t)    return 0;
			if (trait_of(ret) != io_res_t) return 0;
			bool_t res = AcceptEx (
				par ->tcp.tcp,
				tcp ->tcp    ,
				buf		     ,
				0		     ,
				len			 ,
				len			 ,
				&len	     ,
				&ret->res
			);

			if (!res && WSAGetLastError() != ERROR_IO_PENDING) {
				del (ret);
				del (tcp);
				return 0;
			}

			fut   *fut = io_res_fut(ret);
			await (fut);
			del   (fut);
			del   (ret);
			return tcp ;
}

fut*
	tcp_acpt_run
		(tcp_acpt* par)								 {
			if (!par)						 return 0;
			if (trait_of(par) != tcp_acpt_t) return 0;

			return async(this_sched(), tcp_acpt_run_do, par);
}