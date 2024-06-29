#ifndef NET_IOCP_TCP_H
#define NET_IOCP_TCP_H

#include "../../io.h"
#include "end.h"

struct io_sched;

extern obj_trait* tcp_t;
typedef struct    tcp  {
	obj		head;
	u64_t	flag;
	io_run* run;
	any_t   ioc;
	any_t   tcp;
}	tcp;

bool_t tcp_open   (tcp*, obj_trait*)    ;
fut*   tcp_conn   (tcp*, end*)		    ;
void   tcp_close  (tcp*)				;

fut*   tcp_send   (tcp*, u8_t*, u64_t)  ;
fut*   tcp_recv   (tcp*, u8_t*, u64_t)  ;

#endif