#include "io_sched.h"
#include "thd.h"

obj_trait __io_sched_trait	    = {
	.on_new	  = &__io_sched_new   ,
	.on_clone = &__io_sched_clone ,
	.on_ref   =				    0 ,
	.on_del   = &__io_sched_del   ,
	.size	  = sizeof(__io_sched)
};

void* 
	__io_task_run(__io_task* par)								    {
		__sched_susp(&__thd_curr()->sched, __thd_curr()->sched.curr);
		u64_t ret = par->ret; mem_del(0, par);
		return	    par->ret;
}

bool_t 
	__io_sched_new
		(__io_sched* par_sched, u32_t par_count, va_list par) {
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
}

void
	__io_sched_run
		(__io_sched* par)	          {
			DWORD	    task_ret      ;
			__io_sched *task_key = par;
			__io_task  *task		  ;
			 
			while(GetQueuedCompletionStatus(par->io_sched, &task_ret, &task_key, &task, 1)) {
				if(!task->ret) 
					task->ret = task_ret;

				__sched_resm(&__thd_curr()->sched, task->task);
			}
}