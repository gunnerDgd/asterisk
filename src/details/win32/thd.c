#include "thd.h"
#include "curr.h"

#include <Windows.h>

obj_trait __thd_trait     = {
    .on_new   = &__thd_new  ,
    .on_clone = &__thd_clone,
    .on_ref   =            0,
    .on_del   = &__thd_del  ,
    .size     = sizeof(__thd)
};

void
    __thd_main
        (__thd* par)                                                             {
            if (!make_at       (&par->sched   , sched_t)    from(0))       return;
            if (!make_at       (&par->io_sched, io_sched_t) from(0))       return;
            if (!sched_dispatch(&par->sched   , par->func, par->func_arg)) return;

            curr_thd = par;
            while (sched_run(&par->sched))          {
                while (io_sched_run(&par->io_sched));
            }
}

bool_t
    __thd_new
        (__thd* par_thd, u32_t par_count, va_list par) {
            par_thd->func     = va_arg(par, void*);
            par_thd->func_arg = va_arg(par, void*);
            par_thd->thd      = CreateThread(0, 0, __thd_main, par_thd, 0, 0);
            if (par_thd->thd == INVALID_HANDLE_VALUE) {
                del(&par_thd->sched);
                return false_t;
            }

            return true_t;
}

bool_t 
    __thd_clone
        (__thd* par, __thd* par_clone) {
            return false_t;
}

void
    __thd_del
        (__thd* par)             {
            if (!par)      return;
            if (!par->thd) return;

            WaitForSingleObject(par->thd, INFINITE);
            del(&par->sched)   ;
            del(&par->io_sched);
}