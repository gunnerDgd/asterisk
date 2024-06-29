#ifndef IO_H
#define IO_H

#include <thread.h>

#include "io/io_run.h"
#include "io/vma.h"

extern obj_trait *io_t;
struct            io { u8_t io[2 KB]; };

extern thd_local struct io io;

io_run* this_io_run();

#define run_async()                                                      \
    int run_async_do();                                                  \
    int run         ()                                                  {\
        fut *run = async ((void*(*)(void*))run_async_do, null_t);        \
        for ( ; fut_poll(run) == fut_pend ; io_run_flush(this_io_run()));\
        u64_t  ret = (u64_t) fut_ret(run);                               \
        del   (run);                                                     \
        return ret;                                                      \
    }                                                                    \
    int run_async_do()                                                   \

#endif