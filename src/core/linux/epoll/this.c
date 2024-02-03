#include "this.h"
#include "task.h"

__thread this* curr = 0;

obj_trait this_trait =     {
    .on_new   = &this_new  ,
    .on_clone = &this_clone,
    .on_ref   = 0          ,
    .on_del   = &this_del  ,
    .size     = sizeof(this)
};

obj_trait* this_t = &this_trait;

void*
    this_start
        (task* par_task, this* par)         {
            curr      = par                 ;
            par->task = par_task            ;
            void* ret = par->entry(par->arg);

            del   (par);
            return ret ;
}

bool_t       
    this_new
        (this* par_task, u32_t par_count, va_list par)                     {
            thd   *thd   = 0; if (par_count > 0) thd   = va_arg(par, void*);
            void  *entry = 0; if (par_count > 1) entry = va_arg(par, void*);
            void  *arg   = 0; if (par_count > 2) arg   = va_arg(par, void*);
            if (!entry)                 return false_t;
            if (!thd)                   return false_t;
            if (trait_of(thd) != thd_t) return false_t;
            
            io_sched *io_sched = &thd->io_sched; 
            sched    *sched    = &thd->sched   ;
            fut      *fut      = sched_dispatch (sched, this_start, par_task);
            if (!fut)                   return false_t;
            if (trait_of(fut) != fut_t) return false_t;

            par_task->io_sched = io_sched;
            par_task->sched    = sched   ;
            par_task->entry    = entry   ;
            par_task->arg      = arg     ;
            par_task->thd      = thd     ;
            par_task->fut      = fut     ;
            return true_t;
}

bool_t       
    this_clone
        (this* par, this* par_clone) {
            return false_t;
}

void       
    this_del
        (this* par) {
}

sched* 
    this_sched()                              {
        if (!curr)                    return 0;
        if (trait_of(curr) != this_t) return 0;
        return curr->sched;
}

io_sched* 
    this_io_sched()                           {
        if (!curr)                    return 0;
        if (trait_of(curr) != this_t) return 0;
        return curr->io_sched;
}

task* 
    this_task()                               {
        if (!curr)                    return 0;
        if (trait_of(curr) != this_t) return 0;
        return curr->task;
}

thd* 
    this_thd()                                {
        if (!curr)                    return 0;
        if (trait_of(curr) != this_t) return 0;
        return curr->thd;
}

void   
    await_all_va
        (u32_t par_count, va_list par)               {
            for (u32_t i = 0 ; i < par_count ; ++i)  {
                fut *wait = va_arg(par, fut*)        ;
                if (!wait)                   continue;
                if (trait_of(wait) != fut_t) continue;

                await(wait);
            }
}

void   
    await_all
        (u32_t par_count, ...)      {
            va_list  par            ;
            va_start(par, par_count); await_all_va(par_count, par);
            va_end  (par)           ;
}

void*
    await
        (fut* par)                                    {
            if    (!par)                      return 0;
            if    (trait_of(par) != fut_t)    return 0;
            
            if    (fut_poll(par) != fut_pend) return fut_ret(par);
            while (fut_poll(par) == fut_pend) yield();
            return fut_ret (par);
}

fut*
    async
        (void(*par)(void*), void* par_arg)                              {
            this *ret = make (this_t) from (3, this_thd(), par, par_arg);
            if  (!ret)                    return 0;
            if  (trait_of(ret) != this_t) return 0;
            return ret->fut;
}

void   
    yield()                                      {
        this* cur = curr; task_yield(this_task());
        curr       = cur;
}
