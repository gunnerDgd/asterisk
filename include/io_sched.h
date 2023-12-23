#ifndef __IO_SCHED_H__
#define __IO_SCHED_H__

#include <obj.h>

extern obj_trait* io_sched_t;
typedef struct    io_sched { u8_t io_sched[256]; } io_sched;

bool_t            io_sched_run (io_sched*);
io_sched*         io_sched_curr()         ;

#endif