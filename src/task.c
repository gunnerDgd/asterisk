#include "task.h"
#include "sched.h"

#include "details/sched.h"
#include "details/task.h"
#include "details/curr.h"

task  
    task_run
        (void (*par)(void*), void* par_arg) { 
            if (!par)        return 0;
            if (!curr_sched) return 0;

            return __sched_dispatch(curr_sched, par, par_arg);
}

void* 
    task_wait(task par)          { 
        if (!par)        return 0;
        if (!curr_sched) return 0;

        void*  ret = __task_wait(par); __task_free(par);
        return ret;
}

void  
    task_yield() { 
        if(curr_sched) __task_yield(task_curr()); 
}

task  
    task_curr () { 
        return (curr_sched) ? curr_sched->curr : 0;  
}