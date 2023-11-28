#include "thd.h"

#include <Windows.h>
#include <process.h>

__declspec(thread) __thd *thd = 0;

obj_trait __thd_trait      = {
    .on_new   = &__thd_new   ,
    .on_clone = &__thd_clone ,
    .on_ref   =            0 ,
    .on_del   = &__thd_del   ,
    .size     = sizeof(__thd)
};

__thd*   
    __thd_curr() {
        return thd;
}

void
    __thd_main
        (__thd* par) {
            thd = par;
            if (!make_at     (&par->sched, &__sched_trait)    from(0)) return false_t;
            if (!make_at     (&par->sched, &__io_sched_trait) from(0)) return false_t;
            if (!__sched_exec(&par->sched, par->func, par->func_arg))  return false_t;

            while (__sched_run(&par->sched));
}

bool_t
    __thd_new
        (__thd* par_thd, u32_t par_count, va_list par) {
            par_thd->func     = va_arg(par, void*);
            par_thd->func_arg = va_arg(par, void*);

            par_thd->thd = _beginthreadex(0, 0, __thd_main, par_thd, 0, 0);
            if (par_thd->thd == -1) {
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