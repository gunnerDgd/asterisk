#include "io_sched.h"
#include "io_task.h"

#include "sched.h"
#include "task.h"
#include "curr.h"

obj_trait io_sched_trait	 = {
	.on_new	  = &io_sched_new  ,
	.on_clone = &io_sched_clone,
	.on_ref   =				  0,
	.on_del   = &io_sched_del  ,
	.size	  = sizeof(io_sched)
};

obj_trait* io_sched_t = &io_sched_trait;

bool_t 
	io_sched_new
		(io_sched* par_sched, u32_t par_count, va_list par)			       {
			if (par_count)									return false_t ;
			if (!make_at(&par_sched->none, list_t) from(0)) goto new_failed;
			if (!make_at(&par_sched->pend, list_t) from(0)) goto new_failed;
			
			par_sched->io_sched = CreateIoCompletionPort (
				INVALID_HANDLE_VALUE,
				NULL				,
				par_sched			,
				0
			);
			
			if (!par_sched->io_sched) goto new_failed;
			return true_t;
	new_failed:
			del (&par_sched->none);
			del (&par_sched->pend);

			return false_t;
}

bool_t
	io_sched_clone
		(io_sched* par, io_sched* par_clone) { 
			return false_t; 
}

void   
	io_sched_del 
		(io_sched* par)			      { 
			CloseHandle(par->io_sched); 
			del		   (&par->none)   ;
			del		   (&par->pend)   ;
}

bool_t
	io_sched_run
		(io_sched* par)				{
			OVERLAPPED_ENTRY cq[128]; 
			u32_t			 cq_idx ;
			io_task		    *task	;

			bool_t ret = GetQueuedCompletionStatusEx (par->io_sched, cq, 128, &cq_idx, 1, FALSE);
			if   (!ret) return false_t;

			for (u32_t idx = 0 ; idx < cq_idx; ++idx)		{
				if (cq[idx].lpCompletionKey != par) continue;
				if (cq[idx].lpOverlapped    == 0)   continue;

				task		= (u8_t*)cq[idx].lpOverlapped - offsetof(io_task, io_task);
				task->state = io_task_term;
				task->ret    = cq[idx].dwNumberOfBytesTransferred;

				if (task->task)
					task_resm(task->task);
			}

			return cq_idx == 128;
}

io_task*
	io_sched_dispatch
		(io_sched* par)			    {
			if(!curr_sched) return 0;

			io_task  *ret = list_pop_front(&par->none)  ;
			if (!ret) ret = make(io_task_t) from(1, par);
			if (!ret)  return 0;

			return ret;
}