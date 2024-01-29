#include "core.h"

#include <sched.h>
#include "this.h"
#include "thd.h"

void* heap_new(mem* par, u64_t par_size) { return malloc (par_size); }
void  heap_del(mem* par, void* par_del)  { free		    (par_del) ; }
mem   heap           = {
	.on_new = &heap_new,
	.on_del = &heap_del
};

bool_t
	core_new()			  {
	    set_mem(&heap)	  ;
		if (!ws2_new())   {
			ws2_del()     ;
			return false_t;
		}

		return true_t;
}

void
	core_del()    {
		set_mem(0);
		ws2_del() ;
}

int main()					  {
	if (!core_new()) return -1;
	thd		*thd = make (thd_t) from (2, async_main, 0);
	del     (thd);
	core_del()   ;
	return 0;
}