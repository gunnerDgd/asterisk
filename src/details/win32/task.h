#ifndef __DETAILS_WIN32_TASK_H__
#define __DETAILS_WIN32_TASK_H__

#include <obj.h>
#include <cpu.h>
#include <obj_list.h>

#define __task_exec 0
#define __task_susp 1
#define __task_free 2

extern obj_trait __task_trait;
typedef struct   __task      {
    obj             head     ;
    cpu             cpu      ;

    struct __sched *sched    ;
    obj_list_elem   sched_hnd;
    u64_t           state    ;
    void           *stack    ;

    void           *ret      ;
    obj_list        wait_task;
}   __task;

bool_t __task_new  (__task*, u32_t, va_list);
bool_t __task_clone(__task*, __task*)       ;
void   __task_del  (__task*)                ;

void   __task_main (__task*, void*(*)(void*), void*);
void   __task_yield(__task*)                        ;
void*  __task_wait (__task*)                        ;

#endif