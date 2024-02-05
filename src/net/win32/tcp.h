#ifndef __NET_WIN32_TCP_H__
#define __NET_WIN32_TCP_H__

#include "../../core.h"
#include "net.h"
#include "end.h"

#include <task.h>
#include <fut.h>

extern obj_trait* tcp_t;
typedef struct    tcp  {
	obj		  head  ;
	io_sched *sched ;
	u64_t     flag  ;
	HANDLE    tcp_io;
	SOCKET    tcp   ;
	end	      end   ;
}	tcp;

bool_t tcp_new    (tcp*, u32_t, va_list);
bool_t tcp_clone  (tcp*, tcp*)		    ;
void   tcp_del    (tcp*)			    ;

bool_t tcp_open   (tcp*, obj_trait*)    ;
fut*   tcp_conn   (tcp*, end*)		    ;
void   tcp_close  (tcp*)				;

fut*   tcp_send   (tcp*, u8_t*, u64_t)  ;
fut*   tcp_recv   (tcp*, u8_t*, u64_t)  ;

#endif