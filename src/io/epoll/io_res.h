#ifndef __EPOLL_IO_RES_H__
#define __EPOLL_IO_RES_H__

#include <core.h>

extern obj_trait *io_res_t;
typedef struct    io_res  {
    obj   head;
    u64_t stat;
    obj  *dev ;

    u8_t *buf;
    u64_t len;
    any_t arg;
    u64_t ret;
}   io_res;

bool_t io_res_new  (io_res*, u32_t, va_list);
bool_t io_res_clone(io_res*, io_res*)       ;
void   io_res_del  (io_res*)                ;

#endif