#include "task.h"
#include "sched.h"
#include "thd.h"

#include <Windows.h>
#include <stdio.h>

obj_trait __task_trait      = {
    .on_new   = &__task_new   ,
    .on_clone = &__task_clone ,
    .on_ref   =             0 ,
    .on_del   = &__task_del   ,
    .size     = sizeof(__task)
};

void
    __task_main
        (__task* par, void*(*par_fn)(void*), void* par_arg) {
            par->state = __task_exec; par->ret = par_fn(par_arg);
            par->state = __task_free;

            obj_list_for(&par->wait_task, wait_it)  {
                __task* wait = obj_list_get(wait_it);
                __sched_resm(par->sched, wait);
            }

            cpu_switch(&par->cpu, &par->sched->cpu);
}

bool_t
    __task_new
        (__task* par_task, u32_t par_count, va_list par) {
            par_task->stack = VirtualAlloc (
                0             ,
                1 << 20       ,
                MEM_COMMIT    ,
                PAGE_READWRITE
            );

            if (!par_task->stack)                                   return  false_t;
            if (!make_at(&par_task->cpu      , cpu_t)      from(0)) goto new_failed;
            if (!make_at(&par_task->wait_task, obj_list_t) from(0)) goto new_failed;

            par_task->sched     = 0;
            par_task->sched_hnd = 0;

            return true_t;
    new_failed:
            VirtualFree(par_task->stack, 0, MEM_RELEASE);
            return false_t;
}

bool_t 
    __task_clone
        (__task* par, __task* par_clone) {
            return false_t;
}

void
    __task_del
        (__task* par)                                             {
            if(par->stack) VirtualFree(par->stack, 0, MEM_RELEASE);
            del(&par->cpu)      ;
            del(&par->wait_task);
}

void   
    __task_yield(__task* par)                              {
        if (par->sched      != &__thd_curr()->sched) return;
        if (par->sched->thd !=  __thd_curr())        return;

        cpu_switch(&par->cpu, &par->sched->cpu); 
}

void* 
    __task_wait(__task* par)                               {
        if (par->sched      != &__thd_curr()->sched) return;
        if (par->sched->thd !=  __thd_curr())        return;
        if(!par->sched->curr)                        return;

        obj_list_elem wait = obj_list_push_back(&par->wait_task, par->sched->curr);
        __sched_susp    (par->sched    , par->sched->curr);
        obj_list_pop_at(&par->wait_task, wait)            ;

        return par->ret;
}