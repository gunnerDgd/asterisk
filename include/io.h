#ifndef __IO_H__
#define __IO_H__

#include "io/io_sched.h"

extern obj_trait *io_t;
struct            io { u8_t io[2 KB]; };

#ifdef PRESET_FEATURE_THREAD
#ifdef PRESET_COMPILER_GCC
__thread           struct io io;
#elif  PRESET_COMPILER_MSVC
__declspec(thread) struct io io;
#endif
#else
struct io io;
#endif

io_sched* this_io_sched();

#endif