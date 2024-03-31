#include "io_sched.h"
#include "io_poll.h"

#include "core.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

obj_trait io_sched_trait = make_trait (
    io_sched_new    ,
    io_sched_clone  ,
    null_t          ,
    io_sched_del    ,
    sizeof(io_sched),
    null_t
);

obj_trait* io_sched_t = &io_sched_trait;

u64_t
    io_sched_do_poll
        (io_sched* par)									     {
            if (trait_of(par) != io_sched_t) return fut_err  ;
            if (par->pend)                   return fut_ready;
            par->pend = epoll_wait                           (
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
		(io_sched* par)                                      {
            if (trait_of(par) != io_sched_t) return 0        ;
            if (!par->pend)                  return par->pend;
            u64_t ret = 0;

            for (u64_t i = 0 ; i < par->pend ; ++i)      {
                io_poll *poll = par->res[i].data.ptr     ;
                if (trait_of(poll) != io_poll_t) continue;

                poll->poll = par->res[i];
                ++ret;
            }

            par->pend = 0;
            return ret;
}

fut_ops io_sched_fut_ops = make_fut_ops (
    io_sched_do_poll,
    io_sched_do_ret
);

bool_t 
	io_sched_new
		(io_sched* par_run, u32_t par_count, va_list par)                         {
            par_run->hnd = epoll_create(128); if (par_run->hnd < 0) return false_t;
            mem_set(par_run->res, 0x00, sizeof(par_run->res));
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
			close(par->hnd);
}

fut* 
	io_sched_fut
		(io_sched* par)							     {
			if (trait_of(par) != io_sched_t) return 0;
			return make (fut) from                   (
				2				 ,
				&io_sched_fut_ops,
				par
			);
}

u64_t
    io_sched_run
        (io_sched* par)                                      {
            if (trait_of(par)         != io_sched_t) return 0;
            if (io_sched_do_poll(par) != fut_ready)  return 0;
            return io_sched_do_ret(par);
}