#include "tcp_acpt.h"

obj_trait tcp_acpt_trait = make_trait (
	tcp_acpt_new    ,
	tcp_acpt_clone  ,
	null_t          ,
	tcp_acpt_del    ,
	sizeof(tcp_acpt),
	null_t
);

obj_trait* tcp_acpt_t = &tcp_acpt_trait;

bool_t 
	tcp_acpt_new
		(tcp_acpt* par_acpt, u32_t par_count, va_list par)							   {
			io_sched *sched = null_t; if (par_count > 0) sched = va_arg(par, io_sched*);
			end		 *end   = null_t; if (par_count > 1) end   = va_arg(par, void*)	   ;
			if (trait_of(sched) != io_sched_t) return false_t;
			if (trait_of(end)   != end_t)      return false_t;

			if (!make_at (&par_acpt->tcp, tcp) from (1, sched)) return false_t;
			if (!clone_at(&par_acpt->end, end))					return false_t;

			if (!tcp_open(&par_acpt->tcp, end_af(end)))			return false_t;
			if (bind  (par_acpt->tcp.tcp, &end->all, end->len)) return false_t;
			if (listen(par_acpt->tcp.tcp, -1))					return false_t;

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

u64_t
	tcp_acpt_run_do_poll
		(io_res* par)														  {
			if (trait_of(par) != io_res_t) return fut_err; tcp* tcp = par->ret;
			if (trait_of(tcp) != tcp_t)	   return fut_err;

			if (par->stat == fut_pend) io_sched_run(par->sched);
			if (par->stat == fut_ready)						   {
				if (par->arg) drop(par->arg);
				par->arg = null_t;
			}

			return par->stat;
}

void*
	tcp_acpt_run_do_ret
		(io_res* par)														  {
			if (trait_of(par) != io_res_t) return fut_err; tcp* tcp = par->ret;
			if (trait_of(tcp) != tcp_t)	   return fut_err;
			return par->ret;
}

fut_ops tcp_acpt_run_do = make_fut_ops (
	tcp_acpt_run_do_poll,
	tcp_acpt_run_do_ret
);

fut*
	tcp_acpt_run
		(tcp_acpt* par)																			 		      {
			if (trait_of(par) != tcp_acpt_t) return   null_t; tcp    *ret = make (tcp)    from (1, par->sched);
			if (trait_of(ret) != tcp_t)      goto run_failed; io_res *res = make (io_res) from (1, par->sched);
			if (trait_of(res) != io_res_t)   goto run_failed;
			u8_t   *buf = new  (u8_t[128]);
			u64_t   len = 128;
			
			if (!tcp_open(ret, end_af(&par->end))) goto run_failed;
			bool_t  run = AcceptEx								  (
				par->tcp.tcp,
				ret->tcp    ,
				buf			,
				0			,
				64			,
				64			,
				&len		,
				&res->res
			);

			fut *fut = obj_new (null_t, fut_t, 2, &tcp_acpt_run_do, res);
			if (!run && WSAGetLastError() != ERROR_IO_PENDING)			{
				res->stat = fut_err			 ;
				res->ret  = WSAGetLastError();
				drop  (buf);
				del   (ret);
				del   (res);
				return fut ;
			}
			
			res->ret = ret;
			res->arg = buf;
			del   (res);
			return fut ;
	run_failed:
			drop    (buf);
			del		(ret);
			del		(res);
			return null_t;
}