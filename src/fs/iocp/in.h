#ifndef FS_IOCP_IN_H
#define FS_IOCP_IN_H

#include "../../io.h"

extern obj_trait* in_t;
typedef struct    in  {
    obj     head;
    io_run *run;
    void   *ioc;
    void   *dev;
    u64_t   pos;
}   in;

bool_t in_open (in*, const char*);
bool_t in_new  (in*, const char*);
void   in_close(in*);

fut*   in_read (in*, any_t, u64_t);
u64_t  in_seek (in*, u64_t)       ;
u64_t  in_pos  (in*)              ;

#endif
