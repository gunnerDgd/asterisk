#include "io_sched.h"
#include "env/io_sched.h"

obj_trait* io_sched_t = &__io_sched_trait;

void 
	io_sched_run
		(io_sched* par)					    {
			if(trait_of(par) != io_sched_t)
				return false_t;

			__io_sched_run(par);
}

void 
	io_sched_dispatch
		(io_sched* par, task* par_task) {
			if(trait_of(par)      != io_sched_t) return false_t;
			if(trait_of(par_task) != task_t)     return false_t;

			__io_sched_dispatch(par, par_task);
}