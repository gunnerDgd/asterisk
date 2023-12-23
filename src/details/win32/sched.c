#include "sched.h"
#include "task.h"
#include "thd.h"

#include "curr.h"

obj_trait sched_trait     = {
    .on_new   = &sched_new  ,
    .on_clone = &sched_clone,
    .on_ref   =            0,
    .on_del   = &sched_del  ,
    .size     = sizeof(sched)
};

obj_trait* sched_t = &sched_trait;

bool_t  
    sched_new
        (sched* par_sched, u32_t par_count, va_list par)                           {
            mem_res* res = (par_count == 1) ? va_arg(par, mem_res*) : get_mem_res();
            if (!res)             return false_t;
            if (!res->on_new)     return false_t;
            if (!res->on_mem_new) return false_t;
            if (!res->on_del)     return false_t;
            if (!res->on_mem_del) return false_t;

            if (!make_at(&par_sched->cpu , cpu_t)  from(0))      goto new_failed;
            if (!make_at(&par_sched->exec, list_t) from(1, res)) goto new_failed;
            if (!make_at(&par_sched->susp, list_t) from(1, res)) goto new_failed;
            if (!make_at(&par_sched->free, list_t) from(1, res)) goto new_failed;

            par_sched->curr = 0;
            return true_t;

    new_failed:
            del(&par_sched->cpu) ;
            del(&par_sched->exec);
            del(&par_sched->susp);
            del(&par_sched->free);

            return false_t;
}

bool_t
    sched_clone
        (sched* par, sched* par_clone) {
            return false_t;
}

void    
    sched_del
        (sched* par)                                       {
            list_for(&par->susp , susp) del(list_get(susp));
            list_for(&par->free , free) del(list_get(free));
            del     (&par->exec);
            del     (&par->susp);
            del     (&par->free);
}

struct task*
    sched_dispatch
        (sched* par, void(*par_fn)(void*), void* par_arg)  {
            task*     ret = list_pop_front(&par->free) ;
            if (!ret) ret = make (task_t) from (1, par);
            if (!ret) return 0;

            cpu_set_reg64(&ret->cpu, cpu_rip, task_main)                    ;
            cpu_set_reg64(&ret->cpu, cpu_rcx, ret)                          ;
            cpu_set_reg64(&ret->cpu, cpu_rdx, par_fn)                       ;
            cpu_set_reg64(&ret->cpu, cpu_r8 , par_arg)                      ;
            cpu_set_reg64(&ret->cpu, cpu_rsp, (u64_t)ret->stack + 1 mb - 40);
            cpu_set_reg64(&ret->cpu, cpu_rbp, (u64_t)ret->stack + 1 mb - 40);

            ret->sched_hnd = list_push_back(&ret->sched->exec, ret);
            if (ret->sched->curr)                                 {
                ret->sup     = ret->sched->curr                   ;
                ret->sub_hnd = list_push_back(&ret->sup->sub, ret);
            }

            ret->state = task_state_exec;
            ret->ret   = 0;
            ret->wait  = 0;

            return ret;
}

bool_t  
    sched_run
        (sched* par)                                                    {
            if (!par)                     return false_t                ;
            if (trait_of(par) != sched_t) return false_t                ;
            if (list_empty(&par->exec))   return !list_empty(&par->susp);

            while (sched_run_one(par));
            return true_t;
}

bool_t
    sched_run_one 
        (sched* par)                                    {
            if (!par)                     return false_t;
            if (trait_of(par) != sched_t) return false_t;
            if (list_empty(&par->exec))   return false_t;

            sched    *curr   = curr_sched;
            list_elem run_it = list_begin(&par->exec); if (!run_it) return false_t;
            task     *run    = list_get  (run_it)    ; if (!run)    return false_t;

            curr_sched = par;
            par->curr  = run; 
            cpu_switch(&par->cpu, &run->cpu);

            switch(run->state)                                                        {
                case task_state_term: list_pop(&par->exec, run->sched_hnd)            ; break;
                case task_state_susp: list_pop(&par->exec, run->sched_hnd)            ;
                                      run->sched_hnd = list_push_back(&par->susp, run);
                                      
                                      break;
                case task_state_exec: break;
                default             : return false_t;
            }

            curr_sched = curr;
            par->curr  = 0   ;
            return true_t;
}