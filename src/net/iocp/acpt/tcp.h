#ifndef __NET_IOCP_ACPT_TCP_H__
#define __NET_IOCP_ACPT_TCP_H__

#include <core.h>

struct io_res;
struct acpt;
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
        (struct acpt*);

void
    do_tcp_close
        (struct acpt*);

fut*
    do_tcp_fut
        (struct acpt*);

#endif
