#ifndef __CORE_WIN32_IO_SCHED_H__
#define __CORE_WIN32_IO_SCHED_H__

#include "core.h"

#include <obj.h>
#include <list.h>
#include <fut.h>

extern obj_trait* io_sched_t   ;
typedef struct    io_sched	   {
	obj                head    ;
	struct epoll_event res[128];
    u64_t              pend;
	fut               *fut ;
	int                hnd ;
}	io_sched;

bool_t io_sched_new  (io_sched*, u32_t, va_list);
bool_t io_sched_clone(io_sched*, io_sched*)     ;
void   io_sched_del  (io_sched*)				;
fut*   io_sched_fut  (io_sched*)				;

#endif