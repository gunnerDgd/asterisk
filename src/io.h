#ifndef __IO_ALL_H__
#define __IO_ALL_H__

#include "io/io_res.h"
#include "io/io_sched.h"
#include "io/io_poll.h"


extern obj_trait *io_t;
struct            io  {
    obj      head ;
    io_sched sched;
};

bool_t io_new  (struct io*, u32_t, va_list);
bool_t io_clone(struct io*, struct io*)    ;
bool_t io_ref  (struct io*)                ;
void   io_del  (struct io*)                ;

io_sched* this_io_sched();

#ifdef PRESET_FEATURE_THREAD
#ifdef PRESET_COMPILER_GCC
extern __thread           struct io io;
#elif  PRESET_COMPILER_MSVC
extern __declspec(thread) struct io io;
#endif
#else
struct io io;
#endif

#endif