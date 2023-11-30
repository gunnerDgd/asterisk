#ifndef __TASK_H__
#define __TASK_H__

#include <obj.h>

typedef void* task;

task  task_run  (void(*)(void*), void*);
void* task_wait (task)                 ;
void  task_yield()                     ;
task  task_curr ()                     ;

#endif