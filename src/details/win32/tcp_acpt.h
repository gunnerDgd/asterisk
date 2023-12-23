#ifndef __ENV_WIN32_TCP_ACPT_H__
#define __ENV_WIN32_TCP_ACPT_H__

#include "io_sched.h"
#include "v4.h"

extern obj_trait* tcp_acpt_t;
typedef struct    tcp_acpt  {
	obj		  head	  ;
	SOCKET    tcp	  ;
	HANDLE	  tcp_iocp;
	io_sched* io_sched;
}	tcp_acpt;

bool_t		 tcp_acpt_new   (tcp_acpt*, u32_t, va_list);
bool_t		 tcp_acpt_clone (tcp_acpt*, tcp_acpt*)     ;
void		 tcp_acpt_del   (tcp_acpt*)				   ;

bool_t		 tcp_acpt_conn  (tcp_acpt*, v4*)		   ;
void		 tcp_acpt_close (tcp_acpt*)				   ;

void*		 tcp_acpt_run_do(tcp_acpt*)				   ;
struct task* tcp_acpt_run   (tcp_acpt*)				   ;

#endif