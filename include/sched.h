#ifndef __SCHED_H__
#define __SCHED_H__

#include "task.h"

extern obj_trait* sched_t;
typedef struct    sched { u8_t sched[1024]; } sched;

task   sched_dispatch(sched*, void(*)(void*), void*);
bool_t sched_run     (sched*)                       ;
bool_t sched_run_one (sched*)                       ;
sched* sched_curr    ()                             ;

#endif