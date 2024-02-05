#ifndef __UDP_H__
#define __UDP_H__

#include <fut.h>
#include "end.h"

extern obj_trait* udp_t;
typedef struct    udp { u8_t udp[128]; } udp;

bool_t udp_open     (udp*, obj_trait*)        ;
bool_t udp_conn     (udp*, end*)              ;
void   udp_close    (udp*)                    ;

fut*   udp_send     (udp*, u8_t*, u64_t)      ;
fut*   udp_send_to  (udp*, u8_t*, u64_t, end*);

fut*   udp_recv     (udp*, u8_t*, u64_t)      ;
fut*   udp_recv_from(udp*, u8_t*, u64_t, end*);

#endif