#ifndef __DETAILS_WIN32_THD_H__
#define __DETAILS_WIN32_THD_H__

#include "sched.h"
#include "io_sched.h"

extern obj_trait* thd_t  ;
typedef struct    thd    {
    obj      head        ;
    void*    thd         ;
    void   (*func)(void*);
    void    *func_arg    ;
    sched    sched       ;
    io_sched io_sched    ;
}   thd;

void     thd_main (thd*)                ;
bool_t   thd_new  (thd*, u32_t, va_list);
bool_t   thd_clone(thd*, thd*)          ;
void     thd_del  (thd*)                ;

#endif