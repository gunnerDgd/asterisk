#ifndef __ENV_WIN32_IO_SCHED_H__
#define __ENV_WIN32_IO_SCHED_H__

#include "lib.h"

#include <obj.h>
#include <list.h>

extern obj_trait* io_sched_t;
typedef struct    io_sched	{
	obj	  head		;
	void* io_sched	;
	list  none, pend;
}	io_sched;

bool_t		    io_sched_new     (io_sched*, u32_t, va_list);
bool_t		    io_sched_clone   (io_sched*, io_sched*)     ;
void		    io_sched_del     (io_sched*)				;
bool_t		    io_sched_run     (io_sched*)				;
struct io_task* io_sched_dispatch(io_sched*)				;

#endif