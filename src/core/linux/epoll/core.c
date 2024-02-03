#include "core.h"
#include "heap.h"

#include "sched.h"


bool_t
	core_new()		  {
	    set_mem(&heap);
		return true_t ;
}

void
	core_del()    {
		set_mem(0);
}