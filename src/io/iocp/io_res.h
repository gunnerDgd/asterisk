#ifndef __IOCP_IO_RES_H__
#define __IOCP_IO_RES_H__

#include <core.h>
#include "io_sched.h"

extern obj_trait* io_res_t;
typedef struct    io_res  {
	obj		   head ;
	io_sched  *sched;
	u64_t	   stat ;
	OVERLAPPED res  ;
	void*	   ret  ;
	void*	   arg  ;
}	io_res;

bool_t io_res_new  (io_res*, u32_t, va_list);
bool_t io_res_clone(io_res*, io_res*)       ;
void   io_res_del  (io_res*)				;
fut*   io_res_fut  (io_res*)				;

#endif