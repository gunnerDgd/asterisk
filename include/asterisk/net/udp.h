#ifndef __UDP_H__
#define __UDP_H__

#include "v4.h"
#include <fut.h>

obj_trait*     udp_t;
typedef struct udp { u8_t udp[128]; } udp;

bool_t udp_conn        (udp*, obj*)              ;
bool_t udp_conn_v4     (udp*, v4 *)              ;
void   udp_close       (udp*)                    ;

fut*   udp_send        (udp*, u8_t*, u64_t)      ;
fut*   udp_send_to     (udp*, u8_t*, u64_t, obj*);
fut*   udp_send_to_v4  (udp*, u8_t*, u64_t, v4*) ;

fut*   udp_recv        (udp*, u8_t*, u64_t)      ;
fut*   udp_recv_from   (udp*, u8_t*, u64_t, obj*);
fut*   udp_recv_from_v4(udp*, u8_t*, u64_t, v4*) ;

#endif