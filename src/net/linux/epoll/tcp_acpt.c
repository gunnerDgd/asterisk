#include "tcp_acpt.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

obj_trait tcp_acpt_trait     = {
	.on_new	  = &tcp_acpt_new  ,
	.on_clone = &tcp_acpt_clone,
	.on_ref   =				  0,
	.on_del	  = &tcp_acpt_del  ,
	.size	  = sizeof(tcp_acpt)
};

obj_trait* tcp_acpt_t = &tcp_acpt_trait;

i64_t
    tcp_acpt_do_run
        (tcp_acpt* par, u8_t* par_buf, u64_t par_len, tcp* par_tcp)  {
            if (!par)                            return -1      ;
            if (trait_of(par)     != tcp_acpt_t) return -1      ;
            if (trait_of(par_tcp) != tcp_t)      return -1      ;
            if (!io_poll_in(&par->tcp.poll))     goto   run_pend;

            int res = accept (par->tcp.tcp, 0, 0);
            if (res < 0)                                                       goto run_err;
            if (!make_at(&par_tcp->poll, io_poll_t) from (2, par->sched, res)) goto run_err;
            par_tcp->tcp = res;
            errno        = 0  ;
            return par_tcp;
    run_err :
            close    (res);
            errno = EFAULT;
            return -1;
    run_pend:
            errno = EAGAIN;
            return 0;
}

bool_t 
	tcp_acpt_new
		(tcp_acpt* par_acpt, u32_t par_count, va_list par)						  {
			io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
			if (!sched)					            sched = this_io_sched()       ;
			if (!sched)					                         return false_t;
			if (trait_of(sched) != io_sched_t)                   return false_t;
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
		(tcp_acpt* par, end* par_end)						                     {
			if (!par_end)						                   return false_t;
			if (!par)						                       return false_t;
			if (trait_of(par_end) != end_t)		                   return false_t;
			if (trait_of(par)     != tcp_acpt_t)                   return false_t;
            if (!tcp_open(&par->tcp, end_af(par_end)))             return false_t;
			if (bind  (par->tcp.tcp, &par_end->all, par_end->len)) return false_t;
			if (listen(par->tcp.tcp, -1))                          return false_t;
			return true_t;
}

void
	tcp_acpt_close
		(tcp_acpt* par)	                           {
		    if (!par)                        return;
		    if (trait_of(par) != tcp_acpt_t) return;
		    tcp_close(&par->tcp);
}

fut*
	tcp_acpt_run
		(tcp_acpt* par)								 {
			if (!par)						 return 0;
			if (trait_of(par) != tcp_acpt_t) return 0;
            tcp    *tcp = make (tcp_t)    from (1, par->sched); if (trait_of(tcp) != tcp_t) return 0;
            io_res *ret = make (io_res_t) from (
                5              ,
                par            ,
                tcp_acpt_do_run,
                0              ,
                1              ,
                tcp
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret); del (ret);
            if (trait_of(fut) != fut_t)    return 0;
            return fut;
}