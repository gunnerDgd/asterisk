#ifndef __NET_IOCP_UDP_H__
#define __NET_IOCP_UDP_H__

#include "end.h"
#include <core.h>

struct io_sched;

extern obj_trait* udp_t;
typedef struct    udp     {
    obj              head ;
    struct io_sched *sched;
    u32_t            flag;
    any_t            ioc;
    any_t            udp;
}   udp;

bool_t udp_new      (udp*, u32_t, va_list)    ;
bool_t udp_clone    (udp*, udp*)              ;
void   udp_del      (udp*)                    ;

bool_t udp_open     (udp*, obj_trait*)        ;
bool_t udp_conn     (udp*, end*)              ;
void   udp_close    (udp*)                    ;

fut*   udp_send     (udp*, u8_t*, u64_t)      ;
fut*   udp_send_to  (udp*, u8_t*, u64_t, end*);

fut*   udp_recv     (udp*, u8_t*, u64_t)      ;
fut*   udp_recv_from(udp*, u8_t*, u64_t, end*);

#endif