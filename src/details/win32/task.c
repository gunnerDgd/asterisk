#include "task.h"
#include "sched.h"
#include "thd.h"

#include <Windows.h>

obj_trait task_trait      = {
    .on_new   = &task_new   ,
    .on_clone = &task_clone ,
    .on_ref   =           0 ,
    .on_del   = &task_del   ,
    .size     = sizeof(task)
};

obj_trait *task_t = &task_trait;

void
    task_main
        (task* par, void*(*par_fn)(void*), void* par_arg) { 
            par->ret = par_fn(par_arg); 
            if (par->wait) task_resm(par->wait);

            list_while(&par->sub, sub_it)   {
                task *sub = list_get(sub_it); sub_it = list_next(sub_it);
                if  (!sub) continue;

                task_wait(sub);
                task_free(sub);
            }

            par->state = task_state_term;
            cpu_switch(&par->cpu, &par->sched->cpu);
}

bool_t
    task_new
        (task* par_task, u32_t par_count, va_list par)                   {
            if (!make_at(&par_task->cpu, cpu_t)  from(0)) goto new_failed;
            if (!make_at(&par_task->sub, list_t) from(0)) goto new_failed;

            par_task->sched     = va_arg(par, sched*);
            par_task->sched_hnd = 0;
            par_task->sub_hnd   = 0;
            par_task->sup       = 0;

            par_task->ret       = 0;
            par_task->wait      = 0;
            
            par_task->state     = task_state_free;
            par_task->stack     = VirtualAlloc (0, 1 mb, MEM_COMMIT, PAGE_READWRITE);

            if (!par_task->stack) goto new_failed;
            return true_t;
    new_failed:
            del(&par_task->cpu);
            del(&par_task->sub);

            VirtualFree(par_task->stack, 0, MEM_RELEASE);
            return false_t;
}

bool_t 
    task_clone
        (task* par, task* par_clone) {
            return false_t;
}

void
    task_del
        (task* par)                                        {
            if(par->stack)                                 {
                VirtualFree(par->stack, 0   , MEM_RELEASE) ;
                VirtualFree(par->stack, 1 mb, MEM_DECOMMIT);
            }

            del(&par->cpu);
            del(&par->sub);
}

bool_t
    task_yield
        (task* par)                                                {
            if (par->state       != task_state_exec) return false_t;
            if (par->sched->curr != par)             return false_t;

            cpu_switch(&par->cpu, &par->sched->cpu);
            return true_t;
}

void* 
    task_wait
        (task* par)                              {
            if (!par)                    return 0;
            if (trait_of(par) != task_t) return 0;

            if(!par->sched->curr)              return 0;
            if (par->sched->curr == par)       return 0;
            if (par->state == task_state_free) return 0;
            if (par->wait)                     return 0;
              
            par->wait = par->sched->curr; while (par->state != task_state_term) task_susp(par->sched->curr);
            par->wait = 0;
            return par->ret;
}

bool_t
    task_susp
        (task* par)                                          {
            if (!par)                          return false_t;
            if (!par->sched)                   return false_t;
            if (par->state != task_state_exec) return false_t;
            if (par->sched->curr == par)    {
                par->state = task_state_susp; cpu_switch(&par->cpu, &par->sched->cpu);
                par->state = task_state_exec;

                return true_t;
            }

            list_pop(&par->sched->exec, par->sched_hnd)            ;
            par->sched_hnd = list_push_back(&par->sched->susp, par);
            par->state     = task_state_susp;

            return true_t;
}

bool_t
    task_resm
        (task* par)                                    {
            if (!par)                    return false_t;
            if (trait_of(par) != task_t) return false_t;
            
            if (!par->sched)                         return false_t;
            if (par->state       != task_state_susp) return false_t;
            if (par->sched->curr == par)             return false_t;
                        
            list_pop(&par->sched->susp, par->sched_hnd)            ;
            par->sched_hnd = list_push_back(&par->sched->exec, par);
            par->state     = task_state_exec                       ;

            return true_t;
}

bool_t   
    task_free
        (task* par)                                    {
            if (!par)                    return false_t;
            if (trait_of(par) != task_t) return false_t;

            if (par->state       != task_state_term) return false_t;
            if (par->sched->curr == par)             return false_t;
            if (par->sup)                            list_pop(&par->sup->sub, par->sub_hnd);

            u64_t ret      = par->ret       ; list_push_back(&par->sched->free, par);
            par->state     = task_state_free;
            par->sched_hnd = 0;
            par->sub_hnd   = 0;
            par->sup       = 0;

            par->ret       = 0;
            par->wait      = 0;

            return true_t;
}