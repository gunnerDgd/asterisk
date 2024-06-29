#ifndef IO_H
#define IO_H

#include "io/io_res.h"
#include "io/io_sched.h"
#include "io/io_poll.h"
#include "io/vma.h"

#include <thread.h>

extern obj_trait *io_t;
struct            io  {
    obj    head;
    io_run run;
};

io_run* this_io_run();

extern thd_local struct io io;

#endif