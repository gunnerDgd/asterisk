#ifndef __IOCP_IN_H__
#define __IOCP_IN_H__

#include <core.h>
#include <collections.h>
#include "../../io.h"

extern obj_trait* in_t;
typedef struct    in  {
    obj       head ;
    io_sched *sched;
    void     *ioc;
    void     *dev;
    u64_t     pos;
}   in;

bool_t in_new        (in*, u32_t, va_list);
bool_t in_clone      (in*, in*)           ;
void   in_del        (in*)                ;

bool_t in_create_cstr(in*, const char*);
bool_t in_create     (in*, str*);

bool_t in_open_cstr  (in*, const char*);
bool_t in_open       (in*, str*);
void   in_close      (in*);

fut*   in_read       (in*, any_t, u64_t);
u64_t  in_seek       (in*, u64_t)       ;
u64_t  in_pos        (in*)              ;

#endif
