#ifndef __CORE_WIN32_THIS_H__
#define __CORE_WIN32_THIS_H__

#include <obj.h>

#include <sched.h>
#include <task.h>

#include <fut.h>

extern obj_trait *this_t ;
typedef struct    this   {
    obj     head         ;
    sched  *sched        ;
    task   *task         ;
    fut    *fut          ;
    void* (*entry)(void*);
    void   *ret          ;
    void   *arg          ;
}   this;

extern __declspec(thread) this* this_thd;
bool_t this_new    (this*, u32_t, va_list);
bool_t this_clone  (this*, this*)         ;
void   this_del    (this*)                ;

sched* this_sched  ();
task*  this_task   ();

void   await_all_va(u32_t, va_list)       ;
void   await_all   (u32_t, ...)           ;
void*  await       (fut*)                 ;
fut*   async       (void(*)(void*), void*);
void   yield       ()                     ;

#endif