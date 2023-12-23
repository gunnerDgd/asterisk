#ifndef __ENV_WIN32_UDP_H__
#define __ENV_WIN32_UDP_H__

#include "v4.h"

extern obj_trait* udp_t          ;
typedef struct    udp            {
    obj              head        ;
    SOCKET           udp         ;
    HANDLE           udp_io_sched;
    struct io_sched* io_sched    ;
    u32_t            flag        ;
}   udp;

bool_t          udp_new      (udp*, u32_t, va_list)   ;
bool_t          udp_clone    (udp*, udp*)             ;
void            udp_del      (udp*)                   ;

bool_t          udp_conn     (udp*, v4*)              ;
void            udp_close    (udp*)                   ;

struct io_task* udp_send     (udp*, u8_t*, u64_t)     ;
struct io_task* udp_send_to  (udp*, u8_t*, u64_t, v4*);
struct io_task* udp_recv     (udp*, u8_t*, u64_t)     ;
struct io_task* udp_recv_from(udp*, u8_t*, u64_t, v4*);

#endif