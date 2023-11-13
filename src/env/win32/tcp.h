#ifndef __ENV_WIN32_TCP_H__
#define __ENV_WIN32_TCP_H__

#include "io_sched.h"

extern obj_trait __tcp_trait  ;
typedef struct   __tcp        {
	obj			head    ;
	__io_sched* io_sched;
	SOCKET	    tcp	    ;
}	__tcp;

bool_t __tcp_init		  (__tcp*, u32_t, va_list);
bool_t __tcp_init_as_clone(__tcp*, __tcp*)		  ;
void   __tcp_deinit		  (__tcp*)			      ;
u64_t  __tcp_size		  ()					  ;

task* __tcp_conn (__tcp*, const char*, u16_t);
task* __tcp_close(__tcp*);

task* __tcp_send (__tcp*, mem, u64_t);
task* __tcp_recv (__tcp*, mem, u64_t);

#endif