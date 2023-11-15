#ifndef __TCP_ACPT_H__
#define __TCP_ACPT_H__

#include "details/base/task.h"

extern obj_trait* tcp_acpt_t;
typedef struct    tcp_acpt { u8_t tcp_acpt[512]; } tcp_acpt;

bool_t tcp_acpt_conn (tcp_acpt*, const char*, u16_t);
void   tcp_acpt_close(tcp_acpt*);
task*  tcp_acpt_run  (tcp_acpt*);

#endif