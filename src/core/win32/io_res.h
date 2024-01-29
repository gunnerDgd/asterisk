#ifndef __CORE_WIN32_IO_RES_H__
#define __CORE_WIN32_IO_RES_H__

#include <obj.h>
#include <fut.h>

#include "io_run.h"
#include "core.h"

extern obj_trait* io_res_t;
typedef struct    io_res  {
	obj		   head;
	u64_t	   stat;
	io_run    *run ;
	OVERLAPPED res ;
	void*	   ret ;
}	io_res;

bool_t io_res_new  (io_res*, u32_t, va_list);
bool_t io_res_clone(io_res*, io_res*)       ;
void   io_res_del  (io_res*)				;
fut*   io_res_fut  (io_res*)				;

#endif