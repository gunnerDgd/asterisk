#ifndef FS_IOCP_OUT_H
#define FS_IOCP_OUT_H

#include "../../io.h"

extern obj_trait* out_t;
typedef struct    out  {
    obj     head;
    io_run *run;
    void   *ioc;
    void   *dev;
    u64_t   pos;
}   out;

bool_t out_open (out*, const char*);
bool_t out_new  (out*, const char*);
void   out_close(out*);

fut*   out_write(out*, any_t, u64_t);
u64_t  out_seek (out*, u64_t)       ;
u64_t  out_pos  (out*)              ;

#endif
