#ifndef __IO_SCHED_H__
#define __IO_SCHED_H__

#include "base/task.h"

extern obj_trait* io_sched_t;
typedef struct    io_sched { u8_t io_sched[2048]; } io_sched;

void io_sched_run     (io_sched*)		;
void io_sched_dispatch(io_sched*, task*);

#endif