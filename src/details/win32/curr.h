#ifndef __DETAILS_WIN32_CURR_H__
#define __DETAILS_WIN32_CURR_H__

extern __declspec(thread) struct __sched *curr_sched;
extern __declspec(thread) struct __thd   *curr_thd  ;

#endif