#ifndef __TASK_H__
#define __TASK_H__

typedef void* task;

task  task_exec (void(*)(void*), void*);
void* task_wait (task)                 ;
void  task_resm (task)                 ;
void  task_susp (task)                 ;
void  task_yield()                     ;

#define async(par, par_arg) task_exec (par, par_arg)
#define await(par)          task_wait (par)
#define resm(par)           task_resm (par)
#define susp(par)           task_susp (par)
#define yield()             task_yield()


#endif