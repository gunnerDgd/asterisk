#include "curr.h"

#include "task.h"
#include "sched.h"
#include "thd.h"

__declspec(thread) struct __sched *curr_sched;
__declspec(thread) struct __thd   *curr_thd  ;