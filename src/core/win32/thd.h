#ifndef __CORE_WIN32_THD_H__
#define __CORE_WIN32_THD_H__

#include <sched.h>
#include <fut.h>

extern obj_trait* thd_t;
typedef struct    thd  {
    obj     head ;
    sched   sched;
    u64_t   stat ;
    fut    *fut  ;
    void* (*run)(void*);
    void   *arg;
    void   *thd;
    void   *ret;
}   thd;

bool_t thd_new  (thd*, u32_t, va_list);
bool_t thd_clone(thd*, thd*)          ;
void   thd_del  (thd*)                ;

void   thd_wait (thd*)                ;
fut*   thd_fut  (thd*)                ;

#endif