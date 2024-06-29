#include "io_res.h"
#include "io_run.h"

static u64_t
    do_poll
        (io_res* self)                                   {
            if (trait_of(self) != io_res_t) return fut_err;

            if (self->stat == fut_pend) io_run_flush(self->run);
            return self->stat;
}

static any_t
    do_ret
        (io_res* self)                                   {
            if (trait_of(self) != io_res_t) return null_t;
            return self->ret;
}

static fut_ops 
    do_fut = make_fut_ops (
        do_poll,
        do_ret
);

static bool_t 
    do_new
        (io_res* self, u32_t count, va_list arg)                         {
            io_run *run = null_t; if (count > 0) run = va_arg(arg, any_t);
            if (trait_of(run) != io_run_t) return false_t;

            mem_set(&self->res, 0x00, sizeof(OVERLAPPED));
            self->stat = fut_pend;
            self->run  = ref(run);
            self->ret  = 0       ;
            return true_t;
}

static bool_t 
    do_clone
        (io_res* self, io_res* clone) {
            return false_t; 
}

static void
    do_del  
        (io_res* self)     {
            del (self->run);
}

static obj_trait 
    do_obj = make_trait (
        do_new        ,
        do_clone      ,
        null_t        ,
        do_del        ,
        sizeof(io_res),
        null_t
);

obj_trait* io_res_t = &do_obj;

fut*
    io_res_fut
        (io_res* self)                                   {
            if (trait_of(self) != io_res_t) return null_t;
            return make (fut) from        (
                2      ,
                &do_fut,
                self
            );
}