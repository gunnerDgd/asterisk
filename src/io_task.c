#include "io_task.h"
#include "details/io_task.h"
#include "details/curr.h"

void* 
    io_task_wait(io_task par)                                {
        if (!par)                              return false_t;
        if (trait_of(par) != &__io_task_trait) return false_t;
        if (!curr_thd)                         return false_t;
        if (!curr_sched)                       return false_t;

        return __io_task_wait(par);
}