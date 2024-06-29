#ifndef NET_IOCP_ACP_TCP_H
#define NET_IOCP_ACP_TCP_H

#include <core.h>

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
