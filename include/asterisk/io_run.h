#ifndef __IO_RUN_H__
#define __IO_RUN_H__

#include <fut.h>

extern obj_trait* io_run_t;
typedef struct    io_run { u8_t io_run[8 KB]; } io_run;

fut* io_run_fut(io_run*);

#endif