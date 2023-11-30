#ifndef __ENV_WIN32_IO_TASK_H__
#define __ENV_WIN32_IO_TASK_H__

#include <obj.h>
#include "lib.h"

#define __io_task_state_none 0
#define __io_task_state_pend 1
#define __io_task_state_cmpl 2

extern obj_trait __io_task_trait;
typedef struct   __io_task	    {
	obj				   head	    ;
	OVERLAPPED		   io_task  ;
	u64_t			   state	;
	struct __task	  *task	    ;
	struct __io_sched *io_sched ;
	void*			   ret	    ;
}	__io_task;

bool_t __io_task_new  (__io_task*, u32_t, va_list);
bool_t __io_task_clone(__io_task*, __io_task*)    ;
void   __io_task_del  (__io_task*)				  ;
void*  __io_task_wait (__io_task*)				  ;

#endif