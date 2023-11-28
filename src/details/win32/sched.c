#include "sched.h"
#include "task.h"
#include "thd.h"

obj_trait __sched_trait     = {
    .on_new   = &__sched_new  ,
    .on_clone = &__sched_clone,
    .on_ref   =              0,
    .on_del   = &__sched_del  ,
    .size     = sizeof(__sched)
};

bool_t  
    __sched_new
        (__sched* par_sched, u32_t par_count, va_list par)                         {
            mem_res* res = (par_count == 1) ? va_arg(par, mem_res*) : get_mem_res();

            if (!make_at(&par_sched->cpu , cpu_t)      from(0))      return false_t;
            if (!make_at(&par_sched->exec, obj_list_t) from(1, res)) return false_t;
            if (!make_at(&par_sched->susp, obj_list_t) from(1, res)) return false_t;
            if (!make_at(&par_sched->free, obj_list_t) from(1, res)) return false_t;

            par_sched->curr =            0;
            par_sched->thd  = __thd_curr();
            return true_t;
}

bool_t  
    __sched_clone
        (__sched* par, __sched* par_clone) {
            return false_t;
}

void    
    __sched_del
        (__sched* par)             {
            while(__sched_run(par));
            obj_list_for(&par->susp, susp) del(obj_list_get(susp));
            obj_list_for(&par->free, free) del(obj_list_get(free));

            del(&par->exec);
            del(&par->susp);
            del(&par->free);
}

void    
    __sched_susp
        (__sched* par, struct __task* par_task)           {
            if (par             != par_task->sched) return;
            if (par->thd        != __thd_curr())    return;
            if (par_task->state != __task_exec)     return;
            if (par->curr       == par_task)              {
                par_task->state = __task_susp;
                __task_yield(par_task);

                return;
            }

            obj_list_pop_at(&par->exec, par_task->sched_hnd);

            par_task->sched_hnd = obj_list_push_back(&par->susp, par_task);
            par_task->state     = __task_susp;
}

void 
    __sched_resm
        (__sched* par, struct __task* par_task)           {
            if (par             != par_task->sched) return;
            if (par->thd        != __thd_curr())    return;
            if (par_task->state != __task_susp)     return;

            obj_list_pop_at(&par->susp, par_task->sched_hnd);

            par_task->sched_hnd = obj_list_push_back(&par->exec, par_task);
            par_task->state     = __task_exec                             ;
}

struct __task*
    __sched_exec
        (__sched* par, void(*par_fn)(void*), void* par_arg) {
            __task*   ret = obj_list_pop_front(&par->free);
            if (!ret) ret = make(&__task_trait) from(0);
            if (!ret)                          return 0;
            
            ret->sched_hnd = obj_list_push_back(&par->exec, ret);
            if (!ret->sched_hnd) {
                del(ret)      ;
                return false_t;
            }

            cpu_reg_write(&ret->cpu, cpu_rsp, (u64_t)ret->stack + (1 << 20) - 32);
            cpu_reg_write(&ret->cpu, cpu_rbp, (u64_t)ret->stack + (1 << 20) - 32);
            cpu_reg_write(&ret->cpu, cpu_rip, __task_main)                       ;
            cpu_reg_write(&ret->cpu, cpu_rcx, ret)                               ;
            cpu_reg_write(&ret->cpu, cpu_rdx, par_fn)                            ;
            cpu_reg_write(&ret->cpu, cpu_r8 , par_arg)                           ;

            ret->sched = par        ;
            ret->state = __task_exec;
            return ret;
}

bool_t  
    __sched_run(__sched* par)                        {
        if(obj_list_empty(&par->exec)) return false_t;
        obj_list_while(&par->exec, task_it)  {
            par->curr = obj_list_get(task_it);
            if(!par->curr)    {
                par->curr  = 0;
                return false_t;
            }

            cpu_switch(&par->cpu, &par->curr->cpu);
            if (par->curr->state == __task_exec) {
                task_it = obj_list_next(task_it);
                continue;
            }

            task_it = obj_list_next(task_it);
            obj_list_pop_at(&par->exec, par->curr->sched_hnd);

            if (par->curr->state == __task_susp) 
                par->curr->sched_hnd = obj_list_push_back(&par->susp, par->curr);
            if (par->curr->state == __task_free) 
                par->curr->sched_hnd = obj_list_push_back(&par->free, par->curr);
        }

        par->curr = 0;
        return true_t;
}

__task* 
    __sched_curr(__sched* par) {
        return par->curr;
}