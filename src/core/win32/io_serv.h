#ifndef __CORE_WIN32_IO_SERV_H__
#define __CORE_WIN32_IO_SERV_H__

#include "io_run.h"

extern obj_trait *io_serv_t;
typedef struct    io_serv  {
    obj     head;
    io_run *run ;
    fut    *fut ;
}   io_serv;

bool_t io_serv_new  (io_serv*, u32_t, va_list);
bool_t io_serv_clone(io_serv*, io_serv*)      ;
void   io_serv_del  (io_serv*)                ;

#endif