#ifndef __ENV_WIN32_TCP_H__
#define __ENV_WIN32_TCP_H__

#include "v4.h"

extern obj_trait __tcp_trait       ;
typedef struct   __tcp             {
	obj				   head        ;
	struct __io_sched* io_sched    ;
	SOCKET			   tcp	       ;
	HANDLE			   tcp_io_sched;
	__v4			   v4		   ;
}	__tcp;

bool_t			  __tcp_new  (__tcp*, u32_t, va_list);
bool_t			  __tcp_clone(__tcp*, __tcp*)		 ;
void			  __tcp_del  (__tcp*)			     ;

struct __io_task* __tcp_conn (__tcp*, __v4*)		 ;
void              __tcp_close(__tcp*)				 ;
struct __io_task* __tcp_send (__tcp*, u8_t*, u64_t)  ;
struct __io_task* __tcp_recv (__tcp*, u8_t*, u64_t)  ;

#endif