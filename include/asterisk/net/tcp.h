#ifndef __TCP_H__
#define __TCP_H__

#include <fut.h>
#include "v4.h"

extern obj_trait* tcp_t;
typedef struct    tcp { u8_t tcp[128]; } tcp;

fut* tcp_conn   (tcp*, obj*)        ;
fut* tcp_conn_v4(tcp*, v4*)         ;
void tcp_close  (tcp*)              ;

fut* tcp_send   (tcp*, u8_t*, u64_t);
fut* tcp_recv   (tcp*, u8_t*, u64_t);

#endif