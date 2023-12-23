#include "io_task.h"
#include "io_sched.h"

#include "task.h"
#include "sched.h"

#include "curr.h"

obj_trait io_task_trait      = {
	.on_new	  = &io_task_new   ,
	.on_clone = &io_task_clone ,
	.on_ref   = 0              ,
	.on_del   = &io_task_del   ,
	.size	  = sizeof(io_task)
};

obj_trait* io_task_t = &io_task_trait;

bool_t 
    io_task_new
        (io_task* par_task, u32_t par_count, va_list par) {
            par_task->io_sched = va_arg(par, io_sched*) ; 
            if (!par_task->io_sched)                        return false_t;
            if (trait_of(par_task->io_sched) != io_sched_t) return false_t;

            par_task->io_sched = ref(par_task->io_sched);
            par_task->state    = io_task_free           ;
            par_task->ret      = 0                      ;
            par_task->task     = 0                      ;

            return true_t;
}

bool_t 
    io_task_clone
        (io_task* par, io_task* par_clone) { 
            return false_t; 
}

void
    io_task_del  
        (io_task* par)         {
            del (par->io_sched);
}

void*
    io_task_wait 
        (io_task* par)                              {
            if (par->state != io_task_exec) return 0;
            if (!curr_sched)                return 0;
            if (!curr_sched->curr)          return 0;
            if (!curr_thd)                  return 0;

            par->task  = curr_sched->curr; task_susp(par->task); void* ret = par->ret;
            par->state = io_task_free    ;
            par->ret   = 0;
            par->task  = 0;

            list_push_back(&par->io_sched->none, par);
            return ret;
}