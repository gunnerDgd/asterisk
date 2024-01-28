#ifndef __TCP_ACPT_H__
#define __TCP_ACPT_H__

#include <fut.h>
#include "v4.h"

extern obj_trait* tcp_acpt_t;
typedef struct    tcp_acpt { u8_t tcp_acpt[128]; } tcp_acpt;

bool_t tcp_acpt_conn   (tcp_acpt*, obj*);
bool_t tcp_acpt_conn_v4(tcp_acpt*, v4*) ;
void   tcp_acpt_close  (tcp_acpt*)		;
fut*   tcp_acpt_run    (tcp_acpt*)		;

#endif