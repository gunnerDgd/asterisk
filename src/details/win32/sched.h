#ifndef __DETAILS_WIN32_SCHED_H__
#define __DETAILS_WIN32_SCHED_H__

#include <cpu.h>
#include <list.h>

extern obj_trait* sched_t        ;
typedef struct    sched          {
    obj          head            ;
    cpu          cpu             ;
    list         exec, susp, free;
    struct task *curr            ;
}   sched;

bool_t       sched_new     (sched*, u32_t, va_list)       ;
bool_t       sched_clone   (sched*, sched*)               ;
void         sched_del     (sched*)                       ;

struct task* sched_dispatch(sched*, void(*)(void*), void*);
bool_t       sched_run     (sched*)                       ;
bool_t       sched_run_one (sched*)                       ;

#endif