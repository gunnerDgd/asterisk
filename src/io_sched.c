#include "io_sched.h"
#include "details/io_sched.h"
#include "details/thd.h"
#include "details/curr.h"

obj_trait* io_sched_t = &__io_sched_trait;

bool_t 
	io_sched_run
		(io_sched* par)									  {
			if(trait_of(par) != io_sched_t) return false_t;
			if(!par)						return false_t;

			return __io_sched_run(par);
}

io_sched* 
	io_sched_curr()								   {
		return (curr_thd) ? &curr_thd->io_sched : 0;
}