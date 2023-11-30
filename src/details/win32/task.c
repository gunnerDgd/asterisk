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
            par->ret = par_fn(par_arg); 
            if (par->wait) __task_resm(par->wait);

            obj_list_while(&par->child, child_it)     {
                __task *child = obj_list_get(child_it); child_it = obj_list_next(child_it);
                if    (!child) continue;

                __task_wait(child);
                __task_free(child);
            }

            par->state = __task_state_term;
            obj_list_pop_at(&par->sched->exec,  par->sched_hnd) ;
            cpu_switch     (&par->cpu        , &par->sched->cpu);
}

bool_t
    __task_new
        (__task* par_task, u32_t par_count, va_list par)       {
            if (!make_at(&par_task->cpu  , cpu_t)      from(0)) goto new_failed;
            if (!make_at(&par_task->child, obj_list_t) from(0)) goto new_failed;

            par_task->sched      = va_arg(par, __sched*);
            par_task->sched_hnd  = 0;
            par_task->child_hnd  = 0;
            par_task->parent     = 0;

            par_task->ret        = 0;
            par_task->wait       = 0;
            
            par_task->state      = __task_state_free;
            par_task->stack_size = (par_count == 1) ? 1 mb : va_arg(par, u64_t);
            par_task->stack      = VirtualAlloc (
                0                   ,
                par_task->stack_size,
                MEM_COMMIT          ,
                PAGE_READWRITE
            );

            if (!par_task->stack) goto new_failed;
            return true_t;
    new_failed:
            del(&par_task->cpu)  ;
            del(&par_task->child);

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
            del(&par->cpu)  ;
            del(&par->child);
}

bool_t
    __task_yield
        (__task* par)                                                {
            if (par->state       != __task_state_exec) return false_t;
            if (par->sched->curr != par)               return false_t;

            cpu_switch(&par->cpu, &par->sched->cpu);
            return true_t;
}

void* 
    __task_wait
        (__task* par)                                    {
            if(!par->sched->curr)                return 0;
            if (par->sched->curr == par)         return 0;
            if (par->state == __task_state_free) return 0;
            if (par->wait)                       return 0;

            par->wait = par->sched->curr;
            while (par->state != __task_state_term)
                __task_susp(par->sched->curr);

            par->wait = 0;
            return par->ret;
}

bool_t
    __task_susp
        (__task* par)                                          {
            if (!par)                            return false_t;
            if (!par->sched)                     return false_t;
            if (par->state != __task_state_exec) return false_t;

            obj_list_pop_at(&par->sched->exec, par->sched_hnd)         ;
            par->sched_hnd = obj_list_push_back(&par->sched->susp, par);
            par->state     = __task_state_susp;

            if (par->sched->curr == par) cpu_switch(&par->cpu, &par->sched->cpu);
            return true_t;
}

bool_t
    __task_resm
        (__task* par)                                          {
            if (!par)                            return false_t;
            if (!par->sched)                     return false_t;
            if (par->state != __task_state_susp) return false_t;

            
            obj_list_pop_at(&par->sched->susp, par->sched_hnd)         ;
            par->sched_hnd = obj_list_push_back(&par->sched->exec, par);
            par->state     = __task_state_exec                         ;

            return true_t;
}

bool_t   
    __task_free
        (__task* par)                                                {
            if (par->state       != __task_state_term) return false_t;
            if (par->sched->curr == par)               return false_t;
            if (par->parent) obj_list_pop_at(&par->parent->child, par->child_hnd);

            u64_t ret      = par->ret; obj_list_push_back(&par->sched->free, par);
            par->state     = __task_state_free;
            par->sched_hnd = 0;
            par->child_hnd = 0;
            par->parent    = 0;

            par->ret       = 0;
            par->wait      = 0;

            return true_t;
}