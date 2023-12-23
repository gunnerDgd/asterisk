#include "curr.h"

#include "task.h"
#include "sched.h"
#include "thd.h"

__declspec(thread) struct sched *curr_sched = 0;
__declspec(thread) struct thd   *curr_thd   = 0;

struct sched*    sched_curr   () { return  curr_sched; }
struct task*     task_curr    () { return (curr_sched) ? curr_sched->curr : 0; }
struct thd*      thd_curr     () { return  curr_thd  ; }
struct io_sched* io_sched_curr() { return (curr_thd) ? &curr_thd->io_sched : 0; }