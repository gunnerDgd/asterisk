#ifndef __CORE_WIN32_TCP_ACPT_H__
#define __CORE_WIN32_TCP_ACPT_H__

#include "io_run.h"
#include "io_res.h"

#include "tcp.h"
#include "v4.h"

#include <fut.h>

extern obj_trait* tcp_acpt_t;
typedef struct    tcp_acpt  {
	obj		head;
	io_run *run ;
	tcp		tcp ;
}	tcp_acpt;

bool_t tcp_acpt_new    (tcp_acpt*, u32_t, va_list);
bool_t tcp_acpt_clone  (tcp_acpt*, tcp_acpt*)     ;
void   tcp_acpt_del    (tcp_acpt*)				  ;

bool_t tcp_acpt_conn   (tcp_acpt*, obj*)		  ;
bool_t tcp_acpt_conn_v4(tcp_acpt*, v4*)		      ;
void   tcp_acpt_close  (tcp_acpt*)				  ;
fut*   tcp_acpt_run    (tcp_acpt*)				  ;

#endif