#include "io_sched.h"

#include "io_res.h"
#include "fut.h"

#include "core.h"

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
		(io_sched* par_sched, u32_t par_count, va_list par)		 {
			mem_set(par_sched->res, 0x00, sizeof(par_sched->res));
			par_sched->res_count = 0					 ;
			par_sched->stat      = fut_ready			 ;
			par_sched->hnd		 = CreateIoCompletionPort(
				INVALID_HANDLE_VALUE,
				NULL				,
				par_sched			,
				0
			);
			
			if (!par_sched->hnd) return false_t;
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
			CloseHandle(par->hnd); 
}

u64_t
	io_sched_do_poll
		(io_sched* par)									     {
			if (!par)					     return fut_err  ;
			if (trait_of(par) != io_sched_t) return fut_err  ;
			if (par->res_count)				 return fut_ready;

			bool_t res = GetQueuedCompletionStatusEx (
				par->hnd	   ,
				par->res	   ,
				128		       ,
				&par->res_count,
				0			   ,
				FALSE
			);

			if (!res)			 return fut_err ;
			if (!par->res_count) return fut_pend;
			return fut_ready;
}

void*
	io_sched_do_ret
		(io_sched* par)								 {
			if (!par)						 return 0;
			if (trait_of(par) != io_sched_t) return 0;
			if (!par->res_count)			 return 0;

			u64_t ret = 0;
			for (u64_t i = 0 ; i < par->res_count ; ++i)													{
				OVERLAPPED_ENTRY *res_sched_ent = &par->res[i]												;
				io_sched		 *res_sched     = res_sched_ent->lpCompletionKey							;
				io_res			 *res			= (u8_t*)res_sched_ent->lpOverlapped - offsetof(io_res, res);

				if (res_sched != par)				   continue;
				if (!res)							   continue;
				if (trait_of(res_sched) != io_sched_t) continue;
				if (trait_of(res)		!= io_res_t)   continue;
				if (res->stat != fut_pend)			   continue;
				
				if (!res->ret) res->ret = res_sched_ent->dwNumberOfBytesTransferred;
				res->stat = fut_ready;
				ret++;
			}

			return ret;
}

fut_ops io_sched_fut_ops   = {
	.poll = &io_sched_do_poll,
	.ret  = &io_sched_do_ret
};

struct fut* 
	io_sched_fut
		(io_sched* par)								 {
			if (!par)						 return 0;
			if (trait_of(par) != io_sched_t) return 0;
			return make (fut_t) from (
				2				 ,
				&io_sched_fut_ops,
				par
			);
}