#include "task.h"

#include "details/sched.h"
#include "details/task.h"
#include "details/thd.h"

task  
    task_exec
        (void (*par)(void*), void* par_arg)                     {
            __thd   *thd       = __thd_curr(); if(!thd) return 0;
            __sched *thd_sched = &thd->sched ;

            return __sched_exec(thd_sched, par, par_arg);
}



void  
    task_resm
        (task par)                           {
            __thd   *thd       = __thd_curr(); if(!thd) return 0;
            __sched *thd_sched = &thd->sched ;
            __sched_resm(thd_sched, par);
}

void  
    task_susp
        (task par)                           {
            __thd   *thd       = __thd_curr(); if(!thd) return 0;
            __sched *thd_sched = &thd->sched ;
            __sched_susp(thd_sched, par)     ;
}

void* 
    task_wait (task par) { 
        return __task_wait(par); 
}

void 
    task_yield()                         {
        __thd   *thd       = __thd_curr(); if(!thd) return 0;
        __sched *thd_sched = &thd->sched ;

        __task_yield(thd_sched->curr);
}