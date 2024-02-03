#ifndef __CORE_LINUX_EPOLL_IO_POLL_H__
#define __CORE_LINUX_EPOLL_IO_POLL_H__

#include <obj.h>
#include <sys/epoll.h>

extern obj_trait *io_poll_t ;
typedef struct    io_poll   {
    obj                head ;
    struct io_sched   *sched;
    struct epoll_event poll ,
                       mask ;
    int                dev  ;
}   io_poll;

bool_t io_poll_new     (io_poll*, u32_t, va_list);
bool_t io_poll_clone   (io_poll*, io_poll*)      ;
void   io_poll_del     (io_poll*)                ;

void   io_poll_mask_err(io_poll*, bool_t)        ;
void   io_poll_mask_out(io_poll*, bool_t)        ;
void   io_poll_mask_in (io_poll*, bool_t)        ;
bool_t io_poll_hang    (io_poll*)                ;
bool_t io_poll_err     (io_poll*)                ;
bool_t io_poll_out     (io_poll*)                ;
bool_t io_poll_in      (io_poll*)                ;

#endif
