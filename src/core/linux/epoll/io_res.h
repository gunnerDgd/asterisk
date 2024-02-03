#ifndef __CORE_LINUX_EPOLL_IO_RES_H__
#define __CORE_LINUX_EPOLL_IO_RES_H__

#include <obj.h>
#include <fut.h>

extern obj_trait *io_res_t                              ;
typedef i64_t   (*io_res_ops)(obj*, u8_t*, u64_t, void*);
typedef struct    io_res                                {
    obj        head;
    u64_t      stat;
    io_res_ops ops;
    obj       *dev;

    u8_t      *buf;
    u64_t      len;
    void      *arg;
    u64_t      ret;
}   io_res;

bool_t io_res_new  (io_res*, u32_t, va_list);
bool_t io_res_clone(io_res*, io_res*)       ;
void   io_res_del  (io_res*)                ;
fut*   io_res_fut  (io_res*)                ;

#endif
