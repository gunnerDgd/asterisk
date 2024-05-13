#ifndef __IO_H__
#define __IO_H__

#ifdef PRESET_LINUX
#include "epoll/io_poll.h"
#include "epoll/io_res.h"
#include "epoll/io_sched.h"
#elif  PRESET_WIN32
#include "iocp/io_res.h"
#include "iocp/io_sched.h"
#endif

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
__thread           struct io io;
#elif  PRESET_COMPILER_MSVC
__declspec(thread) struct io io;
#endif
#else
struct io io;
#endif

#endif
