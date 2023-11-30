#include "io_task.h"
#include "io_sched.h"

#include "task.h"
#include "sched.h"

#include "curr.h"

obj_trait __io_task_trait     = {
	.on_new	  = __io_task_new   ,
	.on_clone = __io_task_clone ,
	.on_ref   = 0               ,
	.on_del   = __io_task_del   ,
	.size	  = sizeof(__io_task)
};

bool_t 
    __io_task_new
        (__io_task* par_task, u32_t par_count, va_list par)      {
            mem_set(&par_task->io_task, 0x00, sizeof(OVERLAPPED));
            par_task->io_sched = va_arg(par, __io_sched*)        ; if (!par_task->io_sched) return false_t;
            par_task->state    = __io_task_state_none            ;
            par_task->ret      = 0                               ;
            par_task->task     = 0                               ;

            return true_t;
}

bool_t __io_task_clone(__io_task* par, __io_task* par_clone) { return false_t; }
void   __io_task_del  (__io_task* par)                       {}

void*
    __io_task_wait 
        (__io_task* par)                                    {
            if (par->state != __io_task_state_pend) return 0;
            if (!curr_sched)                        return 0;
            if (!curr_sched->curr)                  return 0;
            if (!curr_thd)                          return 0;

            par->task  = curr_sched->curr    ; __task_susp(par->task); void* ret = par->ret;
            par->state = __io_task_state_none;
            par->ret   = 0;
            par->task  = 0;

            obj_list_push_back(&par->io_sched->io_task, par);
            return ret;
}