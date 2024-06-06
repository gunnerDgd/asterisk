#ifndef __FS_LINUX_EPOLL_OUT_H__
#define __FS_LINUX_EPOLL_OUT_H__

#include <core.h>
#include <collections.h>

struct io_sched;

extern obj_trait *out_t;
typedef struct    out     {
    obj              head ;
    struct io_sched *sched;
    int              out;
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
