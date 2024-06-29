#ifndef IO_IO_RUN_H
#define IO_IO_RUN_H

#include <core.h>

extern obj_trait* io_run_t;
typedef struct    io_run { u8_t io_sched[8 KB]; } io_run;

u64_t io_run_flush(io_run*);
fut*  io_run_fut  (io_run*);

#endif