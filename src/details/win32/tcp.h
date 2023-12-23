#ifndef __ENV_WIN32_TCP_H__
#define __ENV_WIN32_TCP_H__

#include "v4.h"

extern obj_trait* tcp_t			 ;
typedef struct    tcp            {
	obj				 head        ;
	struct io_sched* io_sched    ;
	SOCKET			 tcp	     ;
	HANDLE			 tcp_io_sched;
	v4			     v4		     ;
}	tcp;

bool_t			tcp_new    (tcp*, u32_t, va_list);
bool_t			tcp_clone  (tcp*, tcp*)		     ;
void			tcp_del    (tcp*)			     ;

struct task*	tcp_conn   (tcp*, v4*)		     ;
void*			tcp_conn_do(tcp*)			     ;

void            tcp_close  (tcp*)				 ;
struct io_task* tcp_send   (tcp*, u8_t*, u64_t)  ;
struct io_task* tcp_recv   (tcp*, u8_t*, u64_t)  ;

#endif