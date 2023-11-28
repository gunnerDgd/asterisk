#ifndef __DETAILS_WIN32_SCHED_H__
#define __DETAILS_WIN32_SCHED_H__

#include <cpu.h>
#include <obj_list.h>

extern obj_trait __sched_trait     ;
typedef struct   __sched           {
    obj            head            ;
    cpu            cpu             ;
    obj_list       exec, susp, free;
    struct __task *curr            ;
    struct __thd  *thd             ;
}   __sched;

bool_t         __sched_new  (__sched*, u32_t, va_list);
bool_t         __sched_clone(__sched*, __sched*)      ;
void           __sched_del  (__sched*)                ;

void           __sched_susp (__sched*, struct __task*)       ;
void           __sched_resm (__sched*, struct __task*)       ;
struct __task* __sched_exec (__sched*, void(*)(void*), void*);

bool_t         __sched_run  (__sched*)                ;
struct __task* __sched_curr (__sched*)                ;

#endif