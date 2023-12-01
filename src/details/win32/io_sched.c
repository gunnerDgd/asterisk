#include "io_sched.h"
#include "io_task.h"

#include "sched.h"
#include "task.h"
#include "curr.h"

#include <stdio.h>

obj_trait __io_sched_trait	    = {
	.on_new	  = &__io_sched_new   ,
	.on_clone = &__io_sched_clone ,
	.on_ref   =				    0 ,
	.on_del   = &__io_sched_del   ,
	.size	  = sizeof(__io_sched)
};

void* 
	__io_task_main
		(struct __io_task* par)			 {
			__task_susp		  (par->task);
			obj_list_push_back(&par->io_sched->io_task, par); par->task = 0;

			return par->ret;
}

bool_t 
	__io_sched_new
		(__io_sched* par_sched, u32_t par_count, va_list par)					 {
			if (!make_at(&par_sched->io_task, obj_list_t) from(0)) return false_t;
			par_sched->io_sched = CreateIoCompletionPort (
				INVALID_HANDLE_VALUE,
				NULL				,
				par_sched			,
				0
			);
			
			return par_sched->io_sched != 0;
}

bool_t
	__io_sched_clone
		(__io_sched* par, __io_sched* par_clone) { 
			return false_t; 
}

void   
	__io_sched_del 
		(__io_sched* par)			  { 
			CloseHandle(par->io_sched); 
			del       (&par->io_task) ;
}

bool_t
	__io_sched_run
		(__io_sched* par)						  {
			OVERLAPPED_ENTRY cq[128]; u32_t cq_idx;
			__io_task		*task	;

			bool_t ret = GetQueuedCompletionStatusEx (par->io_sched, cq, 128, &cq_idx, 1, FALSE);
			if   (!ret) return false_t;

			for (u32_t idx = 0 ; idx < cq_idx; ++idx)						     {
				task = (u8_t*)cq[idx].lpOverlapped - offsetof(__io_task, io_task);
				if (par != cq[idx].lpCompletionKey)
					continue;

				task->state = __io_task_state_cmpl;
				if (task->ret == 0) task->ret = cq[idx].dwNumberOfBytesTransferred;
				if (task->task)	    __task_resm(task->task);
			}

			return cq_idx == 128;
}

__io_task*
	__io_sched_dispatch
		(__io_sched* par)			{
			if(!curr_sched) return 0;

			__io_task *ret = obj_list_pop_front(&par->io_task)  ;
			if (!ret)  ret = make(&__io_task_trait) from(1, par);
			if (!ret)  return 0;

			return ret;
}