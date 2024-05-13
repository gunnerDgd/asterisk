#ifndef __IO_H__
#define __IO_H__

#include "io/io_sched.h"

extern obj_trait *io_t;
struct            io { u8_t io[2 KB]; };

#ifdef PRESET_FEATURE_THREAD
#ifdef PRESET_COMPILER_GCC
extern __thread           struct io io;
#elif  PRESET_COMPILER_MSVC
extern __declspec(thread) struct io io;
#endif
#else
extern struct io io;
#endif

io_sched* this_io_sched();

#define run_async()                                            \
    int run_async_do();                                        \
    int run         ()                                        {\
        io_sched *sched  = this_io_sched();                    \
        fut      *run_io = io_sched_fut (sched);               \
        fut      *run    = async        (run_async_do, null_t);\
        for ( ; fut_poll(run) == fut_pend ; fut_poll(run_io)); \
    }                                                          \
    int run_async_do()                                         \

#endif