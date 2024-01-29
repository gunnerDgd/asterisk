#ifndef __CORE_WIN32_IO_RUN_H__
#define __CORE_WIN32_IO_RUN_H__

#include "core.h"

#include <obj.h>
#include <list.h>
#include <fut.h>

struct io_task;
extern obj_trait* io_run_t ;
typedef struct    io_run	 {
	obj				 head	 ;
	u64_t			 stat	 ;
	u64_t			 pend    ;
	OVERLAPPED_ENTRY res[128];
	void			*hnd	 ;
}	io_run;

bool_t io_run_new  (io_run*, u32_t, va_list);
bool_t io_run_clone(io_run*, io_run*)       ;
void   io_run_del  (io_run*)				;
fut*   io_run_fut  (io_run*)				;

#endif