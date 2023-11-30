#ifndef __TCP_ACPT_H__
#define __TCP_ACPT_H__

#include "task.h"
#include "io_task.h"

#include "v4.h"

extern obj_trait* tcp_acpt_t;
typedef struct    tcp_acpt { u8_t tcp_acpt[512]; } tcp_acpt;

bool_t  tcp_acpt_conn (tcp_acpt*, obj*);
void    tcp_acpt_close(tcp_acpt*)      ;
io_task tcp_acpt_run  (tcp_acpt*)      ;

#endif