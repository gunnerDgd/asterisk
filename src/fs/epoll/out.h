#ifndef FS_EPOLL_OUT_H
#define FS_EPOLL_OUT_H

#include <core.h>
#include <collections.h>

#include "../../io.h"

extern obj_trait *out_t;
typedef struct    out  {
    obj     head ;
    io_run *run;
    int     out;
}   out;

bool_t out_new  (out*, const char*);
bool_t out_open (out*, const char*);
void   out_close(out*);

fut*   out_write(out*, any_t, u64_t);
u64_t  out_seek (out*, u64_t)       ;
u64_t  out_pos  (out*)              ;

#endif
