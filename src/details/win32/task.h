#ifndef __DETAILS_WIN32_TASK_H__
#define __DETAILS_WIN32_TASK_H__

#include <cpu.h>
#include <obj_list.h>

#define          __task_state_free 0
#define          __task_state_exec 1
#define          __task_state_susp 2
#define          __task_state_term 3
extern obj_trait __task_trait ;
typedef struct   __task       {
    obj             head      ;
    cpu             cpu       ;

    struct __sched *sched     ;
    obj_list_elem   sched_hnd ;
    
    obj_list        child     ;
    obj_list_elem   child_hnd ;
    struct __task  *parent    ;
    
    u64_t           state     ;
    void           *stack     ;
    void           *ret       ;
    struct __task  *wait      ;
}   __task;

void   __task_main (__task*, void*(*)(void*), void*);

bool_t __task_new  (__task*, u32_t, va_list);
bool_t __task_clone(__task*, __task*)       ;
void   __task_del  (__task*)                ;

bool_t __task_yield(__task*);
void*  __task_wait (__task*);
bool_t __task_susp (__task*);
bool_t __task_resm (__task*);
bool_t __task_free (__task*);

#endif