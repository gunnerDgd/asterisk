#ifndef __ENV_WIN32_IO_SCHED_H__
#define __ENV_WIN32_IO_SCHED_H__

#include <Windows.h>
#include <obj.h>

typedef struct __io_task {
	OVERLAPPED     head;
	struct __task* task;
	u64_t	       ret ;
}	  __io_task;
void* __io_task_run(__io_task*);

extern obj_trait __io_sched_trait;
typedef struct   __io_sched      {
	obj   head	  ;
	void* io_sched;
}	__io_sched    ;

bool_t __io_sched_new  (__io_sched*, u32_t, va_list);
bool_t __io_sched_clone(__io_sched*, __io_sched*)   ;
void   __io_sched_del  (__io_sched*)				;
void   __io_sched_run  (__io_sched*)				;

#endif