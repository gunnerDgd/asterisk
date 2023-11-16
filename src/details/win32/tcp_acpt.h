#ifndef __ENV_WIN32_TCP_ACPT_H__
#define __ENV_WIN32_TCP_ACPT_H__

#include "io_sched.h"
#include "v4.h"

extern obj_trait __tcp_acpt_trait;
typedef struct   __tcp_acpt      {
	obj			head	;
	SOCKET      tcp		;
	HANDLE		tcp_iocp;
	__io_sched* io_sched;
}	__tcp_acpt;

bool_t __tcp_acpt_init		   (__tcp_acpt*, u32_t, va_list);
bool_t __tcp_acpt_init_as_clone(__tcp_acpt*,  __tcp_acpt*)  ;
void   __tcp_acpt_deinit	   (__tcp_acpt*)				;
u64_t  __tcp_acpt_size		   ()							;

bool_t __tcp_acpt_conn		   (__tcp_acpt*, __v4*);
void   __tcp_acpt_close		   (__tcp_acpt*)	   ;
task*  __tcp_acpt_run		   (__tcp_acpt*)	   ;

#endif