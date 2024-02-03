#ifndef __CORE_LINUX_EPOLL_THIS_H__
#define __CORE_LINUX_EPOLL_THIS_H__

#include "obj.h"

#include "sched.h"
#include "task.h"

#include "fut.h"

#include "thd.h"
#include "io_sched.h"

extern obj_trait *this_t   ;
typedef struct    this     {
    obj       head         ;
    io_sched *io_sched     ;
    sched    *sched        ;
    task     *task         ;
    thd      *thd          ;
    fut      *fut          ;
    void*   (*entry)(void*);
    void     *ret          ;
    void     *arg          ;
}   this;

extern __thread this* curr;
bool_t    this_new     (this*, u32_t, va_list);
bool_t    this_clone   (this*, this*)         ;
void      this_del     (this*)                ;

thd*      this_thd     ()                     ;
io_sched* this_io_sched()                     ;
sched*    this_sched   ()                     ;
task*     this_task    ()                     ;

void      await_all_va (u32_t, va_list)       ;
void      await_all    (u32_t, ...)           ;
void*     await        (fut*)                 ;
fut*      async        (void(*)(void*), void*);
void      yield        ()                     ;

#endif