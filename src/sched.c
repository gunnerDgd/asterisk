#include "sched.h"
#include "details/sched.h"
#include "details/curr.h"

obj_trait* sched_t = &__sched_trait;


task
    sched_dispatch
        (sched* par, void(*par_func)(void*), void* par_arg) {
            if (!par)      return 0;
            if (!par_func) return 0;

            return __sched_dispatch(par, par_func, par_arg);
}

bool_t sched_run (sched* par) { return (par) ? __sched_run(par) : false_t; }
sched* sched_curr()           { return curr_sched; }