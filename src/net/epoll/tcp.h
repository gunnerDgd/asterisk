#ifndef NET_EPOLL_TCP_H
#define NET_EPOLL_TCP_H

#include <core.h>

#include "../../io.h"
#include "end.h"

extern obj_trait* tcp_t;
typedef struct    tcp  {
	obj     head;
    io_poll poll;
	u64_t   flag;

    io_run *run;
	int     tcp;
}	tcp;

bool_t tcp_open (tcp*, obj_trait*)    ;
fut*   tcp_conn (tcp*, end*)		  ;
void   tcp_close(tcp*)				  ;

fut*   tcp_send (tcp*, u8_t*, u64_t)  ;
fut*   tcp_recv (tcp*, u8_t*, u64_t)  ;

#endif