#ifndef NET_TCP_H
#define NET_TCP_H

#include "end.h"

extern obj_trait* tcp_t;
typedef struct    tcp { u8_t tcp[256]; } tcp;

bool_t tcp_open (tcp*, obj_trait*)  ;
fut*   tcp_conn (tcp*, end*)        ;
void   tcp_close(tcp*)              ;

fut*   tcp_send (tcp*, u8_t*, u64_t);
fut*   tcp_recv (tcp*, u8_t*, u64_t);

#endif