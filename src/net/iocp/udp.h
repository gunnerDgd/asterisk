#ifndef NET_IOCP_UDP_H
#define NET_IOCP_UDP_H

#include "../../io.h"
#include "end.h"

extern obj_trait* udp_t;
typedef struct    udp  {
    obj     head;
    u32_t   flag;
    io_run* run;
    any_t   ioc;
    any_t   udp;
}   udp;

bool_t udp_open     (udp*, obj_trait*)        ;
bool_t udp_conn     (udp*, end*)              ;
void   udp_close    (udp*)                    ;

fut*   udp_send     (udp*, u8_t*, u64_t)      ;
fut*   udp_send_to  (udp*, u8_t*, u64_t, end*);

fut*   udp_recv     (udp*, u8_t*, u64_t)      ;
fut*   udp_recv_from(udp*, u8_t*, u64_t, end*);

#endif