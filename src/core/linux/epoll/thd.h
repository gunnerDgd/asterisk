#ifndef __CORE_LINUX_EPOLL_THD_H__
#define __CORE_LINUX_EPOLL_THD_H__

#include "sched.h"
#include "fut.h"

#include "io_sched.h"

extern obj_trait* thd_t;
typedef struct    thd  {
    obj       head    ;
    io_sched  io_sched;
    sched     sched   ;
    u64_t     stat    ;
    void*   (*run)(void*);
    void     *arg;
    pthread_t thd;
    void     *ret;
}   thd;

bool_t thd_new  (thd*, u32_t, va_list);
bool_t thd_clone(thd*, thd*)          ;
void   thd_del  (thd*)                ;

void   thd_wait (thd*)                ;
fut*   thd_fut  (thd*)                ;

#endif