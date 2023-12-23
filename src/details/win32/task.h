#ifndef __DETAILS_WIN32_TASK_H__
#define __DETAILS_WIN32_TASK_H__

#include <cpu.h>
#include <list.h>

#define           task_state_free 0
#define           task_state_exec 1
#define           task_state_susp 2
#define           task_state_term 3
extern obj_trait* task_t   ;
typedef struct    task     {
    obj           head     ;
    cpu           cpu      ;

    struct sched *sched    ;
    list_elem     sched_hnd;
    
    list          sub    ;
    list_elem     sub_hnd;
    struct task  *sup    ;
    
    u64_t         state    ;
    void         *stack    ;
    void         *ret      ;
    struct task  *wait     ;
}   task;

void   task_main (task*, void*(*)(void*), void*);

bool_t task_new  (task*, u32_t, va_list);
bool_t task_clone(task*, task*)         ;
void   task_del  (task*)                ;

bool_t task_yield(task*);
void*  task_wait (task*);
bool_t task_susp (task*);
bool_t task_resm (task*);
bool_t task_free (task*);

#endif