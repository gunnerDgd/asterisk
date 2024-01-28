#ifndef __CORE_WIN32_TCP_H__
#define __CORE_WIN32_TCP_H__

#include "v4.h"
#include <task.h>
#include <fut.h>

extern obj_trait* tcp_t	   ;
typedef struct    tcp      {
	obj				 head  ;
	struct io_sched* sched ;
	SOCKET			 tcp   ;
	HANDLE			 tcp_io;
	obj				*addr  ;
}	tcp;

bool_t tcp_new    (tcp*, u32_t, va_list);
bool_t tcp_clone  (tcp*, tcp*)		    ;
void   tcp_del    (tcp*)			    ;

fut*   tcp_conn   (tcp*, obj*)		    ;
fut*   tcp_conn_v4(tcp*, v4*)		    ;
void   tcp_close  (tcp*)				;

fut*   tcp_send   (tcp*, u8_t*, u64_t)  ;
fut*   tcp_recv   (tcp*, u8_t*, u64_t)  ;

#endif