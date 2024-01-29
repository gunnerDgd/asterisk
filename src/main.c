#include "core.h"
#include "net.h"

void async_main();
int main	   ()			  {
	if (!core_new()) return -1;
	if (!net_new ()) return -1;
	thd		*thd = make (thd_t) from (2, async_main, 0);
	del     (thd);
	net_del ();
	core_del();
	return 0;
}