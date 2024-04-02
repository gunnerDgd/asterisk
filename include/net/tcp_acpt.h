#ifndef __TCP_ACPT_H__
#define __TCP_ACPT_H__

#include "end.h"

extern obj_trait* tcp_acpt_t;
typedef struct    tcp_acpt { u8_t tcp_acpt[128]; } tcp_acpt;
fut* tcp_acpt_run(tcp_acpt*);

#endif