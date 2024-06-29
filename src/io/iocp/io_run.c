#include "io_run.h"
#include "io_res.h"


static u64_t
	do_poll
		(io_run* self)									    {
			if (trait_of(self) != io_run_t) return fut_err  ;
			if (self->pend)                 return fut_ready;
			bool_t res = GetQueuedCompletionStatusEx	    (
				self->hnd  ,
				self->res  ,
				128		   ,
				&self->pend,
				0		   ,
				FALSE
			);

			if (!res)        return fut_err ;
			if (!self->pend) return fut_pend;
			return fut_ready;
}

static any_t
	do_ret
		(io_run* self)									 {
			if (trait_of(self) != io_run_t) return null_t;
			if (!self->pend)			    return null_t;
			OVERLAPPED_ENTRY *cqe = null_t;
			io_res			 *res = null_t;
			u64_t			  ret = 0;

			for (u64_t i = 0 ; i < self->pend; ++i) {
				cqe = &self->res[i];

				if (cqe->lpCompletionKey != self) continue;
				res = (u8_t*) cqe->lpOverlapped;
				res = (u8_t*) res - offsetof(io_res, res);

				if (trait_of(res) != io_res_t) continue;
				if (res->stat != fut_pend)	   continue;
				
				if (!res->ret) res->ret = cqe->dwNumberOfBytesTransferred;
				res->stat = fut_ready;
				ret++;
			}

			self->pend = 0;
			return ret;
}

static fut_ops 
	do_fut = make_fut_ops (
		do_poll,
		do_ret
);

static bool_t
	do_new
		(io_run* self, u32_t count, va_list arg)       {
			mem_set(self->res, 0x00, sizeof(self->res));
			self->stat = fut_ready;
			self->pend = 0		  ;

			self->hnd  = CreateIoCompletionPort (
				INVALID_HANDLE_VALUE,
				NULL				,
				self				,
				0
			);
			
			if (!self->hnd) return false_t;
			return true_t;
}

static bool_t 
	do_clone
		(io_run* self, io_run* clone) {
			return false_t; 
}

static void   
	do_del  
		(io_run* self)			  {
			CloseHandle(self->hnd);
}

static obj_trait 
	do_obj = make_trait (
		do_new		  ,
		do_clone	  ,
		null_t		  ,
		do_del		  ,
		sizeof(io_run),
		null_t
);

obj_trait* io_run_t = &do_obj;

fut* 
	io_run_fut
		(io_run* self)									 {
			if (trait_of(self) != io_run_t) return null_t;
			return make (fut) from		  (
				2	   ,
				&do_fut,
				self
			);
}

u64_t
	io_run_flush
		(io_run* self)						   		      {
			if (trait_of (self) != io_run_t) return null_t;
			if (do_poll  (self) == fut_pend) return null_t;
			return do_ret(self);
}