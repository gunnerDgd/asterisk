#ifndef __DETAILS_WIN32_CURR_H__
#define __DETAILS_WIN32_CURR_H__

extern __declspec(thread) struct sched *curr_sched;
extern __declspec(thread) struct thd   *curr_thd  ;

struct sched*    sched_curr   ();
struct thd*      thd_curr     ();
struct task*     task_curr    ();
struct io_sched* io_sched_curr();

#endif