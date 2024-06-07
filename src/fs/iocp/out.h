#ifndef __IOCP_OUT_H__
#define __IOCP_OUT_H__

#include <core.h>
#include <collections.h>
#include "../../io.h"

extern obj_trait* out_t;
typedef struct    out  {
    obj       head ;
    io_sched *sched;
    void     *ioc;
    void     *dev;
    u64_t     pos;
}   out;

bool_t out_new        (out*, u32_t, va_list);
bool_t out_clone      (out*, out*)          ;
void   out_del        (out*)                ;

bool_t out_create_cstr(out*, const char*);
bool_t out_create     (out*, str*);

bool_t out_open_cstr  (out*, const char*);
bool_t out_open       (out*, str*);
void   out_close      (out*);

fut*   out_write      (out*, any_t, u64_t);
u64_t  out_seek       (out*, u64_t)       ;
u64_t  out_pos        (out*)              ;

#endif
