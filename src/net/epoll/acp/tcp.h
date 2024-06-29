#ifndef NET_EPOLL_ACP_TCP_H
#define NET_EPOLL_ACP_TCP_H

#include <core.h>

struct io_res;
struct acp;
struct tcp;

u64_t
    do_tcp_poll
        (struct io_res*);

any_t
    do_tcp_ret
        (struct io_res*);

extern fut_ops
    do_tcp;

bool_t
    do_tcp_open
        (struct acp*);

void
    do_tcp_close
        (struct acp*);

fut*
    do_tcp_fut
        (struct acp*);

#endif
