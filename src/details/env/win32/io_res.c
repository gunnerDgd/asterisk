#include "io_res.h"
#include "io_sched.h"

obj_trait __io_res_trait				  = {
	.init		   = &__io_res_init			,
	.init_as_clone = &__io_res_init_as_clone,
	.init_as_ref   = 0					    ,
	.deinit		   = &__io_res_deinit 	    ,
	.name		   = 0					    ,
	.size		   = __io_res_size,
};

bool_t 
	__io_res_init
		(__io_res* par_res, u32_t par_count, va_list par)     {
			par_res->io_sched = ref  (va_arg(par, __io_sched*))										;
			par_res->task     = async(__io_res_main, par_res, sched_curr(&par_res->io_sched->sched));
			par_res->state    = __io_res_idle;

			if(!par_res->task)        {
				del(par_res->io_sched);
				return false_t;
			}

			memset(&par_res->hnd, 0x00, sizeof(OVERLAPPED));
			return true_t;
}

bool_t 
	__io_res_init_as_clone
		(__io_res* par, __io_res* par_clone) {
			return false_t;
}

void   
	__io_res_deinit
		(__io_res* par)       {
			del(par->io_sched);
			del(par->task)    ;
}

u64_t  
	__io_res_size()			   {
		return sizeof(__io_res);
}

void* 
	__io_res_main
		(task* par, __io_res* par_res)			  {
			if (par_res->state == __io_res_pending)
				return 0;
			if (par_res->state == __io_res_idle)  {
				par_res->state  = __io_res_pending;
				susp(par_res->task);
			}

			u64_t  ret = par_res->ret; del(par_res);
			return ret;
}