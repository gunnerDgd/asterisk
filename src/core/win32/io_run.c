#include "io_run.h"
#include "io_res.h"
#include "this.h"

#include "core.h"

obj_trait io_run_trait	   = {
	.on_new	  = &io_run_new  ,
	.on_clone = &io_run_clone,
	.on_ref   =			    0,
	.on_del   = &io_run_del  ,
	.size	  = sizeof(io_run)
};

obj_trait* io_run_t = &io_run_trait;

bool_t 
	io_run_new
		(io_run* par_run, u32_t par_count, va_list par)		 {
			mem_set(par_run->res, 0x00, sizeof(par_run->res));
			par_run->pend = 0						 ;
			par_run->stat = fut_ready				 ;
			par_run->hnd	= CreateIoCompletionPort (
				INVALID_HANDLE_VALUE,
				NULL				,
				par_run				,
				0
			);
			
			if (!par_run->hnd) return false_t;
			return true_t;
}

bool_t 
	io_run_clone
		(io_run* par, io_run* par_clone) {
			return false_t; 
}

void   
	io_run_del  
		(io_run* par)			 {
			CloseHandle(par->hnd);
}

u64_t
	io_run_do_poll
		(io_run* par)									   {
			if (!par)					   return fut_err  ;
			if (trait_of(par) != io_run_t) return fut_err  ;
			if (par->pend)				   return fut_ready;
			bool_t res = GetQueuedCompletionStatusEx	   (
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
	io_run_do_ret
		(io_run* par)							   {
			if (!par)					   return 0;
			if (trait_of(par) != io_run_t) return 0;
			if (!par->pend)				   return 0;

			u64_t ret = 0;
			for (u64_t i = 0 ; i < par->pend; ++i)										{
				OVERLAPPED_ENTRY *ent = &par->res[i]									;
				io_run		     *run = ent->lpCompletionKey							;
				io_res			 *res = (u8_t*)ent->lpOverlapped - offsetof(io_res, res);
				if (run != par)				   continue;
				if (!res)					   continue;
				if (trait_of(run) != io_run_t) continue;
				if (trait_of(res) != io_res_t) continue;
				if (res->stat != fut_pend)	   continue;
				
				if (!res->ret) res->ret = ent->dwNumberOfBytesTransferred;
				res->stat = fut_ready;
				ret++;
			}

			par->pend = 0;
			return ret;
}

fut_ops io_run_fut_ops   = {
	.poll = &io_run_do_poll,
	.ret  = &io_run_do_ret
};

fut* 
	io_run_fut
		(io_run* par)							   {
			if (!par)					   return 0;
			if (trait_of(par) != io_run_t) return 0;
			return make (fut_t) from (
				2				 ,
				&io_run_fut_ops,
				par
			);
}