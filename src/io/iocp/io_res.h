#ifndef IO_IOCP_IO_RES_H
#define IO_IOCP_IO_RES_H

#include <core.h>
#include "io_run.h"

extern obj_trait* io_res_t;
typedef struct    io_res  {
	obj		   head;
	u64_t	   stat;
	io_run    *run;
	OVERLAPPED res;
	void*	   ret;
	void*	   arg;
}	io_res;

fut* io_res_fut(io_res*);

#endif