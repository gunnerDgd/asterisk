#ifndef __IO_SCHED_H__
#define __IO_SCHED_H__

#include <fut.h>

extern obj_trait* io_sched_t;
typedef struct    io_sched { u8_t io_run[8 KB]; } io_sched;

fut* io_sched_fut(io_sched*);

#endif