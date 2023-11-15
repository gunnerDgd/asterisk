#ifndef __ENV_WIN32_IO_RES_H__
#define __ENV_WIN32_IO_RES_H__

#include "sched.h"
#include "task.h"

#include "env.h"

#define __io_res_idle      0
#define __io_res_pending   1
#define __io_res_completed 2

extern obj_trait __io_res_trait; 
typedef struct   __io_res	    {
	obj				   head	    ;
	OVERLAPPED		   hnd	    ;
	task			  *task	    ;
	struct __io_sched *io_sched ;
	u64_t	           ret	    ;
	u64_t			   state    ;
}	__io_res;

void*  __io_res_main		 (task*, __io_res*)			;
bool_t __io_res_init         (__io_res*, u32_t, va_list);
bool_t __io_res_init_as_clone(__io_res*, __io_res*)	    ;
void   __io_res_deinit		 (__io_res*)				;
u64_t  __io_res_size		 ()						    ;

#endif