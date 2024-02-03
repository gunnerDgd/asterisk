#ifndef __CORE_LINUX_EPOLL_IO_DEV_H__
#define __CORE_LINUX_EPOLL_IO_DEV_H__

#include <obj.h>
#include <bit.h>

struct        io_sched ;
typedef union io_stat  {
    struct             {
        u64_t in    : 1;
        u64_t out   : 1;
        u64_t err   : 1;
        u64_t avail : 1;
    };  u64_t all      ;
}   io_stat;

extern obj_trait *io_dev_t;
typedef struct    io_dev  {
    obj              head ;
    struct io_sched *sched;
    io_stat          stat ;
    u64_t            dev  ;
}   io_dev;

bool_t   io_dev_new  (io_dev*, u32_t, va_list);
bool_t   io_dev_clone(io_dev*, io_dev*)       ;
void     io_dev_del  (io_dev*)                ;
io_stat* io_dev_stat (io_dev*)                ;

#endif
