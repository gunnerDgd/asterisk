#include "net.h"
#include "fs.h"
#include "io.h"

int run_async();
int run      ()                                {
#ifdef PRESET_WIN32
    if (!make_at(&net, net) from (0)) return -1;
#endif
    fut*   fut = async(run_async, null_t);
    while (fut_poll(fut) == fut_pend)    {
    }
    return 0;
}