#include "io_run.h"
#include "io_poll.h"

#include "core.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static u64_t
    do_poll
        (io_run* self)                                    {
            if (trait_of(self) != io_run_t) return fut_err;
            if (self->run <= 0)             return fut_err;

            if (self->num) return fut_ready;
            self->num = epoll_wait         (
                self->run,
                self->res,
                128      ,
                0
            );

            if  (!self->num) return fut_pend;
            return fut_ready;
}

static u64_t
    do_ret
		(io_run* self)                              {
            if (trait_of(self) != io_run_t) return 0;
            if (self->run <= 0)             return 0;
            if (!self->num) return self->num;
            u64_t num = self->num;
            u64_t ret = 0;

            for (u64_t i = 0 ; i < num ; ++i)        {
                io_poll *poll = self->res[i].data.ptr;

                if (trait_of(poll) != io_poll_t) continue;
                poll->poll = self->res[i];
                ++ret;
            }

            self->num = 0;
            return ret;
}

static fut_ops
    do_fut = make_fut_ops (
        do_poll,
        do_ret
);

static bool_t
	do_new
		(io_run* self, u32_t count, va_list arg) {
            self->run = epoll_create(128);
            self->num = 0;

            if (self->run < 0) return false_t;
			return true_t;
}

static bool_t
	do_clone
		(io_run* self, io_run* clone) {
			return false_t; 
}

static void
	do_del
		(io_run* self)	    {
			close(self->run);
}

static obj_trait
    do_run = make_trait (
        do_new        ,
        do_clone      ,
        null_t        ,
        do_del        ,
        sizeof(io_run),
        null_t
);

obj_trait* io_run_t = &do_run;

fut* 
    io_run_fut
		(io_run* self)                                   {
			if (trait_of(self) != io_run_t) return null_t;
			if (self->run <= 0)             return null_t;
			return make (fut) from (
				2      ,
				&do_fut,
				self
			);
}

u64_t
    io_run_flush
        (io_run* par)                                {
            if (trait_of (par) != io_run_t)  return 0;
            if (do_poll  (par) != fut_ready) return 0;
            return do_ret(par);
}