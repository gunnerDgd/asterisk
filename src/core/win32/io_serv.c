#include "io_serv.h"
#include "this.h"

obj_trait io_serv_trait     = {
    .on_new   = &io_serv_new  ,
    .on_clone = &io_serv_clone,
    .on_ref   = 0             ,
    .on_del   = &io_serv_del  ,
    .size     = sizeof(io_serv)
};

obj_trait* io_serv_t = &io_serv_trait;

void
    io_serv_run
        (io_serv* par)                            {
            if (!par)                       return;
            if (trait_of(par) != io_serv_t) return;
            for ( ; use_count(par) ; yield())   {
                fut  *run = io_run_fut(par->run);
                await(run);
                del  (run);
            }
}

bool_t 
    io_serv_new
        (io_serv* par_serv, u32_t par_count, va_list par)                 {
            io_run *run = 0; if (par_count > 0) run = va_arg(par, io_run*);
            if (!run)                      return false_t;
            if (trait_of(run) != io_run_t) return false_t;
            par_serv->fut = async(io_serv_run, par_serv);
            par_serv->run = ref(run)                    ;
            return true_t;
}

bool_t 
    io_serv_clone
        (io_serv* par, io_serv* par_clone) {
            return false_t;
}

void   
    io_serv_del
        (io_serv* par)     {
            await(par->fut);
            del  (par->fut);
            del  (par->run);
}