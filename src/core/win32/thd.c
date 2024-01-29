#include "thd.h"
#include "this.h"

#include <Windows.h>

obj_trait thd_trait     = {
    .on_new   = &thd_new  ,
    .on_clone = &thd_clone,
    .on_ref   =          0,
    .on_del   = &thd_del  ,
    .size     = sizeof(thd)
};

obj_trait* thd_t = &thd_trait;

void*
    thd_do_run
        (thd* par)                               {
            if (!par)                   return -1;
            if (trait_of(par) != thd_t) return -1;
            this *this = make (this_t) from      (
                3          ,
                par        ,
                &par->sched,
                par->run   ,
                par->arg
            );

            if (!this)                       return -1;
            if (trait_of(this) != this_t)    return -1;

            while (!sched_idle(&par->sched)) sched_run(&par->sched);
            par->stat = fut_ready;
            return  0;
}


u64_t  
    thd_do_poll
        (thd* par)                                    {
            if (!par)                   return fut_err;
            if (trait_of(par) != thd_t) return fut_err;
            return par->stat;
}

void* 
    thd_do_ret
        (thd* par)                              {
            if (!par)                   return 0;
            if (trait_of(par) != thd_t) return 0;
            return par->ret;
}

fut_ops thd_fut_ops  = {
    .poll = thd_do_poll,
    .ret  = thd_do_ret
};

bool_t
    thd_new
        (thd* par_thd, u32_t par_count, va_list par)                  {
            void* run = 0; if (par_count > 0) run = va_arg(par, void*);
            void* arg = 0; if (par_count > 1) arg = va_arg(par, void*);
            
            if (!make_at(&par_thd->sched, sched_t) from(0)) return false_t;
            if (!run)                                       return false_t;
            par_thd->stat = fut_pend;
            par_thd->run  = run     ;
            par_thd->arg  = arg     ;
            par_thd->thd  = CreateThread (
                0         , 
                0         ,
                thd_do_run,
                par_thd   ,
                0         ,
                0
            );

            if (par_thd->thd == INVALID_HANDLE_VALUE) {
                del (&par_thd->sched);
                return false_t;
            }
            return true_t;
}

bool_t 
    thd_clone
        (thd* par, thd* par_clone) {
            return false_t;
}

void
    thd_del
        (thd* par)               {
            if (!par)      return;
            if (!par->thd) return;

            WaitForSingleObject(par->thd, INFINITE);
            del(&par->sched)   ;
}

void  
    thd_wait 
        (thd* par)                                 {
            if (!par)                        return;
            if (trait_of(par) != thd_t)      return;
            WaitForSingleObject(par->thd, INFINITE);
            del(&par->sched);
}

fut*   
    thd_fut  
        (thd* par)                              {
            if (!par)                   return 0;
            if (trait_of(par) != thd_t) return 0;
            return make (fut_t) from            (
                2           ,
                &thd_fut_ops,
                par
            );
}