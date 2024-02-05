#include "tcp_acpt.h"

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
			if (!sched)							    sched = this_io_sched()		  ;
			if (!sched)											 return false_t;
			if (trait_of(sched) != io_sched_t)					 return false_t;
			if (!make_at(&par_acpt->tcp, tcp_t) from (1, sched)) return false_t;
			if (!make_at(&par_acpt->end, end_t) from (0))		 return false_t;
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
		(tcp_acpt* par, end* par_end)											 {
			if (trait_of(par_end) != end_t)						   return false_t;
			if (trait_of(par)     != tcp_acpt_t)				   return false_t;
			if (!tcp_open(&par->tcp, end_af(par_end)))			   return false_t;
			if (bind  (par->tcp.tcp, &par_end->all, par_end->len)) return false_t;
			if (listen(par->tcp.tcp, -1))						   return false_t;

			par->end.all = par_end->all;
			par->end.len = par_end->len;
			return true_t;
}

void
	tcp_acpt_close
		(tcp_acpt* par)							   {
			if (!par)						 return;
			if (trait_of(par) != tcp_acpt_t) return;
			tcp_close(&par->tcp);
}

void*
	tcp_acpt_run_do
		(tcp_acpt* par)																					      {
			tcp    *tcp = make (tcp_t)    from (1, par->sched); if (trait_of(tcp) != tcp_t)    goto run_failed;
			io_res *ret = make (io_res_t) from (1, par->sched); if (trait_of(ret) != io_res_t) goto run_failed;
			if (!tcp_open(tcp, end_af(&par->end)))			  {
				del		  (ret);
				goto run_failed;
			}

			u8_t    buf[128] = { 0x00, };
			u32_t   len      = 64	   ;
			bool_t  res      = AcceptEx (
				par ->tcp.tcp,
				tcp ->tcp    ,
				buf		     ,
				0		     ,
				64			 ,
				64			 ,
				&len	     ,
				&ret->res
			);

			if (!res && WSAGetLastError() != ERROR_IO_PENDING) {
				del		  (ret);
				goto run_failed;
			}

			fut   *fut = io_res_fut(ret);
			await (fut);
			del   (fut);
			del   (ret);

			getpeername(tcp->tcp, &tcp->end.all, &tcp->end.len);
			return tcp ;
	run_failed:
			del (tcp);
			return  0;
}

fut*
	tcp_acpt_run
		(tcp_acpt* par)								 {
			if (!par)						 return 0;
			if (trait_of(par) != tcp_acpt_t) return 0;

			return async(tcp_acpt_run_do, par);
}