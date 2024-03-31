#include "tcp_acpt.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

obj_trait tcp_acpt_trait = make_trait (
    tcp_acpt_new    ,
    tcp_acpt_clone  ,
    null_t          ,
    tcp_acpt_del    ,
    sizeof(tcp_acpt),
    null_t
);

obj_trait* tcp_acpt_t = &tcp_acpt_trait;

u64_t
    tcp_acpt_do_poll
        (io_res* par)                                                                  {
            if (trait_of(par)  != io_res_t)    return fut_err; tcp_acpt *acpt = par->dev;
            if (trait_of(acpt) != tcp_acpt_t)  return fut_err; tcp      *tcp  = par->arg;
            if (trait_of(tcp)  != tcp_t)       return fut_err;
            if (io_poll_hang(&acpt->tcp.poll)) return fut_err;
            if (io_poll_err (&acpt->tcp.poll)) return fut_err;

            if (!io_poll_in(&acpt->tcp.poll)) io_sched_run(acpt->sched);
            if (!io_poll_in(&acpt->tcp.poll)) return fut_pend;
            tcp->tcp = accept(acpt->tcp.tcp, 0, 0);

            if (!make_at(&tcp->poll, io_poll) from (2, tcp->sched, tcp->tcp)) return fut_err;
            par->ret = (u64_t) tcp;
            return fut_ready;
}

tcp*
    tcp_acpt_do_ret
        (io_res* par)                                                                 {
            if (trait_of(par)  != io_res_t)   return null_t; tcp_acpt *acpt = par->dev;
            if (trait_of(acpt) != tcp_acpt_t) return null_t;
            return (tcp*) par->ret;
}

fut_ops tcp_acpt_do = make_fut_ops (tcp_acpt_do_poll, tcp_acpt_do_ret);

bool_t 
	tcp_acpt_new
		(tcp_acpt* par_acpt, u32_t par_count, va_list par)						       {
			io_sched *sched = null_t; if (par_count > 0) sched = va_arg(par, io_sched*);
			end      *end   = null_t; if (par_count > 1) end   = va_arg(par, void*)    ;
			if (trait_of(sched) != io_sched_t) return false_t;
			if (trait_of(end)   != end_t)      return false_t;

			if (!make_at (&par_acpt->tcp, tcp) from (1, sched)) return false_t;
			if (!tcp_open(&par_acpt->tcp, end_af(end)))         return false_t;
			if (bind  (par_acpt->tcp.tcp, &end->all, end->len)) return false_t;
			if (listen(par_acpt->tcp.tcp, -1))                  return false_t;
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

fut*
	tcp_acpt_fut
		(tcp_acpt* par)								            {
			if (trait_of(par)       != tcp_acpt_t) return null_t;
			if (trait_of(&par->tcp) != tcp_t)      return null_t;

            tcp    *run = make (tcp)    from (1, par->sched)         ;
            io_res *res = make (io_res) from (4, par, null_t, 0, run);
            fut    *ret = make (fut)    from (2, &tcp_acpt_do, res)  ;
            if (trait_of(run) != tcp_t)    return null_t;
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            return ret;
}