#ifndef __DETAILS_WIN32_THD_H__
#define __DETAILS_WIN32_THD_H__

#include "../../sched.h"
#include "../../io_sched.h"
#include "../../task.h"

extern obj_trait __thd_trait;
typedef struct   __thd      {
    obj      head        ;
    void*    thd         ;
    void   (*func)(void*);
    void    *func_arg    ;
    sched    sched       ;
    io_sched io_sched    ;
}   __thd;

void     __thd_main (__thd*)                ;
bool_t   __thd_new  (__thd*, u32_t, va_list);
bool_t   __thd_clone(__thd*, __thd*)        ;
void     __thd_del  (__thd*)                ;

#endif