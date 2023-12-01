#include "sched.h"
#include "task.h"
#include "thd.h"

#include "curr.h"

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

            if (!make_at(&par_sched->cpu , cpu_t)  from(0))      return false_t;
            if (!make_at(&par_sched->exec, list_t) from(1, res)) return false_t;
            if (!make_at(&par_sched->susp, list_t) from(1, res)) return false_t;
            if (!make_at(&par_sched->free, list_t) from(1, res)) return false_t;

            par_sched->curr = 0;
            return true_t;
}

bool_t
    __sched_clone
        (__sched* par, __sched* par_clone) {
            return false_t;
}

void    
    __sched_del
        (__sched* par)                                     {
            list_for(&par->susp , susp) del(list_get(susp));
            list_for(&par->free , free) del(list_get(free));
            del     (&par->exec);
            del     (&par->susp);
            del     (&par->free);
}

struct __task*
    __sched_dispatch
        (__sched* par, void(*par_fn)(void*), void* par_arg)  {
            __task*  ret = list_pop_front(&par->free)        ;
            if(!ret) ret = make (&__task_trait) from (1, par);
            if(!ret) return 0;

            cpu_reg_write(&ret->cpu, cpu_rip, __task_main)                  ;
            cpu_reg_write(&ret->cpu, cpu_rcx, ret)                          ;
            cpu_reg_write(&ret->cpu, cpu_rdx, par_fn)                       ;
            cpu_reg_write(&ret->cpu, cpu_r8 , par_arg)                      ;
            cpu_reg_write(&ret->cpu, cpu_rsp, (u64_t)ret->stack + 1 mb - 40);
            cpu_reg_write(&ret->cpu, cpu_rbp, (u64_t)ret->stack + 1 mb - 40);

            ret->sched_hnd = list_push_back(&ret->sched->exec, ret);
            if (ret->sched->curr)                                        {
                ret->parent    = ret->sched->curr                        ;
                ret->child_hnd = list_push_back(&ret->parent->child, ret);
            }

            ret->state = __task_state_exec;
            ret->ret   = 0;
            ret->wait  = 0;

            return ret;
}

bool_t  
    __sched_run
        (__sched* par)                 {
            if (list_empty(&par->exec)) return !list_empty(&par->susp);
            __sched* prev = curr_sched;

            list_while(&par->exec,   curr_it) {
                par->curr = list_get(curr_it);
                if (!par->curr) continue;

                curr_sched = par                      ;
                cpu_switch(&par->cpu, &par->curr->cpu);

                curr_it = list_next(curr_it);
                if (par->curr->state == __task_state_exec)
                    continue;
                
                if (par->curr->state == __task_state_term) list_pop(&par->exec, par->curr->sched_hnd);
                if (par->curr->state == __task_state_susp)                      {
                    list_pop(&par->exec, par->curr->sched_hnd)                  ;
                    par->curr->sched_hnd = list_push_back(&par->susp, par->curr);
                }
            }

            while(!list_empty(&par->exec))            {
                par->curr = list_pop_front(&par->exec);
                if (!par->curr) continue;

                curr_sched = par;
                cpu_switch(&par->cpu, &par->curr->cpu);
            }
 
            curr_sched = prev; par->curr = 0;
            return true_t;
}