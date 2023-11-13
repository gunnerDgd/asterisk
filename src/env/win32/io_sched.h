#ifndef __ENV_WIN32_IO_SCHED_H__
#define __ENV_WIN32_IO_SCHED_H__

#include "../../base/sched.h"
#include "../../base/task.h"

#include "env.h"

extern obj_trait __io_sched_trait;
typedef struct   __io_sched      {
	obj    head ;
	HANDLE hnd  ;
	sched  sched;
}	__io_sched  ;

bool_t __io_sched_init		   (__io_sched*, u32_t, va_list);
bool_t __io_sched_init_as_clone(__io_sched*, __io_sched*)   ;
void   __io_sched_deinit	   (__io_sched*)				;
u64_t  __io_sched_size		   ()						    ;

void   __io_sched_run	       (__io_sched*)	   ;
void   __io_sched_dispatch     (__io_sched*, task*);

#endif