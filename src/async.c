#include "async.h"
#include "details/task.h"
#include "details/io_task.h"

task  
    async(void(*par)(void*), void* par_arg) {
        if (!par)          return 0;
        if (!task_curr())  return 0;
        if (!sched_curr()) return 0;

        return task_run(par, par_arg);
}

void* 
    await(obj* par)                {
        if (!par)          return 0;
        if (!task_curr())  return 0;
        if (!sched_curr()) return 0;

        if (trait_of(par) == &__task_trait)    return task_wait   (par);
        if (trait_of(par) == &__io_task_trait) return io_task_wait(par);

        return 0;
}