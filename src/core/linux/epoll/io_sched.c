#include "io_sched.h"
#include "io_dev.h"
#include "this.h"

#include "core.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

obj_trait io_sched_trait	 = {
	.on_new	  = &io_sched_new  ,
	.on_clone = &io_sched_clone,
	.on_ref   =			      0,
	.on_del   = &io_sched_del  ,
	.size	  = sizeof(io_sched)
};

obj_trait* io_sched_t = &io_sched_trait;

u64_t
    io_sched_do_poll
        (io_sched* par)									   {
            if (!par)					     return fut_err;
            if (trait_of(par) != io_sched_t) return fut_err;
            if (par->pend)                   return fut_err;
            par->pend = epoll_wait                         (
                par->hnd,
                par->res,
                128     ,
                0
            );

            if  (!par->pend) return fut_pend;
            return fut_ready;
}

u64_t
	io_sched_do_ret
		(io_sched* par)                              {
            if (!par)                        return 0;
            if (trait_of(par) != io_sched_t) return 0;

            u64_t ret = 0;
            for (u64_t i = 0 ; i < par->pend ; ++i)    {
                io_dev *dev = par->res[i].data.ptr     ;
                if (!dev)                      continue;
                if (trait_of(dev) != io_dev_t) continue;
                io_dev_stat(dev)->in = 1;
                ++ret;
            }

            return ret;
}

void*
	io_sched_do
		(io_sched* par)							     {
			if (!par)					     return 0;
			if (trait_of(par) != io_sched_t) return 0;
			for ( ; use_count(par) ; yield())	     {
				io_sched_do_poll(par);
				io_sched_do_ret (par);
			}
			return 0;
}

bool_t 
	io_sched_new
		(io_sched* par_run, u32_t par_count, va_list par)            {
		    int hnd = epoll_create(128); if (hnd <= 0) return false_t;
            mem_set(par_run->res, 0x00, sizeof(par_run->res));
		    par_run->fut = async(io_sched_do, par_run);
		    par_run->hnd = hnd                        ;
			return true_t;
}

bool_t 
	io_sched_clone
		(io_sched* par, io_sched* par_clone) {
			return false_t; 
}

void   
	io_sched_del  
		(io_sched* par)	   {
			await(par->fut);
			del	 (par->fut);
			close(par->hnd);
}

fut_ops io_sched_fut_ops   = {
	.poll = &io_sched_do_poll,
	.ret  = &io_sched_do_ret
};

fut* 
	io_sched_fut
		(io_sched* par)							     {
			if (!par)					     return 0;
			if (trait_of(par) != io_sched_t) return 0;
			return make (fut_t) from (
				2				 ,
				&io_sched_fut_ops,
				par
			);
}