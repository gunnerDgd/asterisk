#ifndef EPOLL_IO_POLL_H
#define EPOLL_IO_POLL_H

#include <core.h>
#include <sys/epoll.h>

extern obj_trait *io_poll_t;
typedef struct    io_poll  {
    obj                head;
    struct epoll_event poll,
                       mask;
    struct io_run     *run;
    int                dev;
}   io_poll;

void   io_poll_mask_err(io_poll*, bool_t);
void   io_poll_mask_out(io_poll*, bool_t);
void   io_poll_mask_in (io_poll*, bool_t);

bool_t io_poll_hang    (io_poll*);
bool_t io_poll_err     (io_poll*);
bool_t io_poll_out     (io_poll*);
bool_t io_poll_in      (io_poll*);

#endif
