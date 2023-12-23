#include "async.h"
#include "curr.h"

task*
    async
        (void(*par)(void*), void* par_arg)  {
            if (!par)               return 0;
            if (!curr_sched)        return 0;
            if (!curr_sched->curr)  return 0;

            return sched_dispatch(curr_sched, par, par_arg);
}

void* 
    await
        (obj* par)                          {
            if (!par)               return 0;
            if (!curr_sched)        return 0;
            if (!curr_sched->curr)  return 0;

            if (trait_of(par) == task_t)    return task_wait   (par);
            if (trait_of(par) == io_task_t) return io_task_wait(par);

            return 0;
}

void  
    await_all
        (u32_t par_count, ...)      {
            va_list  par            ;
            va_start(par, par_count); for (u32_t idx = 0 ; idx < par_count ; ++idx) await(va_arg(par, obj*));
            va_end  (par)           ;
}