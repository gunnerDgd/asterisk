#include "io_sched.h"
#include "io_res.h"
#include "this.h"

#include "core.h"

obj_trait io_sched_trait	 = {
	.on_new	  = &io_sched_new  ,
	.on_clone = &io_sched_clone,
	.on_ref   =			      0,
	.on_del   = &io_sched_del  ,
	.size	  = sizeof(io_sched)
};

obj_trait* io_sched_t = &io_sched_trait;

u64_t
	io_sched_do_poll
		(io_sched* par)									     {
			if (!par)					     return fut_err  ;
			if (trait_of(par) != io_sched_t) return fut_err  ;
			if (par->pend)				     return fut_ready;
			bool_t res = GetQueuedCompletionStatusEx	     (
				par->hnd  ,
				par->res  ,
				128		  ,
				&par->pend,
				0		  ,
				FALSE
			);

			if (!res)		return fut_err ;
			if (!par->pend) return fut_pend;
			return fut_ready;
}

void*
	io_sched_do_ret
		(io_sched* par)							     {
			if (!par)					     return 0;
			if (trait_of(par) != io_sched_t) return 0;
			if (!par->pend)				     return 0;

			u64_t ret = 0;
			for (u64_t i = 0 ; i < par->pend; ++i)										  {
				OVERLAPPED_ENTRY *ent   = &par->res[i]									  ;
				io_sched		 *sched = ent->lpCompletionKey					 	      ;
				io_res			 *res   = (u8_t*)ent->lpOverlapped - offsetof(io_res, res);
				if (sched != par)				   continue;
				if (!res)					       continue;
				if (trait_of(sched) != io_sched_t) continue;
				if (trait_of(res)   != io_res_t)   continue;
				if (res->stat != fut_pend)	       continue;
				
				if (!res->ret) res->ret = ent->dwNumberOfBytesTransferred;
				res->stat = fut_ready;
				ret++;
			}

			par->pend = 0;
			return ret;
}

void*
	io_sched_do
		(io_sched* par)							     {
			if (!par)					     return 0;
			if (trait_of(par) != io_sched_t) return 0;
			for ( ; use_count(par) ; yield())	     {
				io_sched_do_poll(par);
				io_sched_do_ret (par);
			}
			return 0;
}

bool_t 
	io_sched_new
		(io_sched* par_run, u32_t par_count, va_list par)    {
			mem_set(par_run->res, 0x00, sizeof(par_run->res));
			par_run->pend = 0						   ;
			par_run->stat = fut_ready				   ;
			par_run->fut  = async(io_sched_do, par_run);
			par_run->hnd  = CreateIoCompletionPort     (
				INVALID_HANDLE_VALUE,
				NULL				,
				par_run				,
				0
			);
			
			if (!par_run->hnd) return false_t;
			return true_t;
}

bool_t 
	io_sched_clone
		(io_sched* par, io_sched* par_clone) {
			return false_t; 
}

void   
	io_sched_del  
		(io_sched* par)			 {
			await	   (par->fut);
			del		   (par->fut);
			CloseHandle(par->hnd);
}

fut_ops io_sched_fut_ops   = {
	.poll = &io_sched_do_poll,
	.ret  = &io_sched_do_ret
};

fut* 
	io_sched_fut
		(io_sched* par)							     {
			if (!par)					     return 0;
			if (trait_of(par) != io_sched_t) return 0;
			return make (fut_t) from (
				2				 ,
				&io_sched_fut_ops,
				par
			);
}