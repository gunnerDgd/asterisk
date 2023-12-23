#ifndef __ASYNC_H__
#define __ASYNC_H__

#include "task.h"
#include "io_task.h"
#include "sched.h"

task  async    (void(*)(void*), void*);
void* await    (obj*)                 ;
void  await_all(u32_t, ...)           ;

#endif