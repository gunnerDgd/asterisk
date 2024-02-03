#include "core.h"
#include "net.h"

void async_main();
int main	   ()			  {
	if (!core_new()) return -1;
#ifdef PRESET_WIN32
	if (!net_new ()) return -1;
#endif
	thd		*thd = make (thd_t) from (2, async_main, 0);
	del     (thd);
#ifdef PRESET_WIN32
	net_del ();
#endif
	core_del();
	return 0;
}