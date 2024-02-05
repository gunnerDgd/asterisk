#ifndef __TCP_H__
#define __TCP_H__

#include <fut.h>
#include "end.h"

extern obj_trait* tcp_t;
typedef struct    tcp { u8_t tcp[128]; } tcp;

bool_t tcp_open (tcp*, obj_trait*)  ;
fut*   tcp_conn (tcp*, end*)        ;
void   tcp_close(tcp*)              ;

fut*   tcp_send (tcp*, u8_t*, u64_t);
fut*   tcp_recv (tcp*, u8_t*, u64_t);

#endif