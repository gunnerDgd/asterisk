#ifndef __ENV_WIN32_IO_TASK_H__
#define __ENV_WIN32_IO_TASK_H__

#include <obj.h>
#include "lib.h"

#define			  io_task_free 0
#define			  io_task_exec 1
#define			  io_task_term 2
extern obj_trait* io_task_t  ;
typedef struct    io_task	 {
	obj				 head	 ;
	OVERLAPPED		 io_task ;
	u64_t			 state	 ;
	struct task	    *task	 ;
	struct io_sched *io_sched;
	void*			 ret	 ;
}	io_task;

bool_t io_task_new  (io_task*, u32_t, va_list);
bool_t io_task_clone(io_task*, io_task*)      ;
void   io_task_del  (io_task*)				  ;
void*  io_task_wait (io_task*)				  ;

#endif