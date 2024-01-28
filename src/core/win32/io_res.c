#include "io_res.h"

#include "io_sched.h"
#include "fut.h"


obj_trait io_res_trait     = {
	.on_new	  = &io_res_new  ,
	.on_clone = &io_res_clone,
	.on_ref   = 0            ,
	.on_del   = &io_res_del  ,
	.size	  = sizeof(io_res)
};

obj_trait* io_res_t = &io_res_trait;

bool_t 
    io_res_new
        (io_res* par_res, u32_t par_count, va_list par) {
            par_res->sched = va_arg(par, io_sched*);
            if (!par_res->sched)                        return false_t;
            if (trait_of(par_res->sched) != io_sched_t) return false_t;

            par_res->sched = ref(par_res->sched);
            par_res->stat  = fut_pend           ;
            par_res->ret   = 0                  ;
            return true_t;
}

bool_t 
    io_res_clone
        (io_res* par, io_res* par_clone) {
            return false_t; 
}

void
    io_res_del  
        (io_res* par)       {
            del (par->sched);
}

u64_t
    io_res_do_poll
        (io_res* par)                                    {
            if (!par)                      return fut_err;
            if (trait_of(par) != io_res_t) return fut_err;
            return par->stat;
}

void*
    io_res_do_ret
        (io_res* par)                                    {
            if (!par)                      return fut_err;
            if (trait_of(par) != io_res_t) return fut_err;
            return par->ret;
}

fut_ops io_res_fut_ops   = {
    .poll = &io_res_do_poll,
    .ret  = &io_res_do_ret
};

struct fut*
    io_res_fut
        (io_res* par)                              {
            if (!par)                      return 0;
            if (trait_of(par) != io_res_t) return 0;
            return make (fut_t) from               (
                2              ,
                &io_res_fut_ops,
                par
            );
}