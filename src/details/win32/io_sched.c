#include "io_sched.h"
#include "io_res.h"

obj_trait __io_sched_trait				    = {
	.init		   = &__io_sched_init		  ,
	.init_as_clone = &__io_sched_init_as_clone,
	.init_as_ref   =						 0,
	.deinit		   = &__io_sched_deinit		  ,
	.name		   =						 0,
	.size		   = &__io_sched_size
};

bool_t 
	__io_sched_init
		(__io_sched* par_sched, u32_t par_count, va_list par) {
			alloc* par_alloc = (par_count == 0) ? get_alloc() : va_arg(par, alloc*);
			par_sched->hnd   = CreateIoCompletionPort (
				INVALID_HANDLE_VALUE,
				NULL				,
				par_sched			,
				0
			);
			
			if (par_sched->hnd == INVALID_HANDLE_VALUE) 
				return false_t;
			if (!par_alloc)
				return false_t;

			return make_at(par_sched->sched, sched_t) from(1, par_alloc);
}

bool_t 
	__io_sched_init_as_clone
		(__io_sched* par, __io_sched* par_clone) {
			return false_t;
}

void
	__io_sched_deinit
		(__io_sched* par)		    {
			del		   (&par->sched);
			CloseHandle (par->hnd)  ;
}

u64_t
	__io_sched_size() { 
		return sizeof(__io_sched);
}

void
	__io_sched_run
		(__io_sched* par)	   {
			u8_t *ret		   ;
			DWORD ret_bytes    ;
			void *ret_key = par;

			while(sched_run(&par->sched))											  {
			while(GetQueuedCompletionStatus(par->hnd, &ret_bytes, &ret_key, &ret, 1)) {
				__io_res* res = ret - offsetof(__io_res, hnd);
				if(res->state == __io_res_pending)				  {
					res->ret   = (res->ret) ? res->ret : ret_bytes;
					res->state = __io_res_completed;
					resm(res->task);
				}
			}
			}
}

void
	__io_sched_dispatch
		(__io_sched* par, task* par_task) {
			sched_dispatch(&par->sched, par_task);
}