#ifndef __ENV_WIN32_UDP_H__
#define __ENV_WIN32_UDP_H__

#include "v4.h"

extern obj_trait __udp_trait       ;
typedef struct   __udp             {
    obj                head        ;
    SOCKET             udp         ;
    HANDLE             udp_io_sched;
    struct __io_sched* io_sched    ;
    u32_t              flag        ;
}   __udp;

bool_t            __udp_new      (__udp*, u32_t, va_list)     ;
bool_t            __udp_clone    (__udp*, __udp*)             ;
void              __udp_del      (__udp*)                     ;

bool_t            __udp_conn     (__udp*, __v4*)              ;
void              __udp_close    (__udp*)                     ;

struct __io_task* __udp_send     (__udp*, u8_t*, u64_t)       ;
struct __io_task* __udp_send_to  (__udp*, u8_t*, u64_t, __v4*);
struct __io_task* __udp_recv     (__udp*, u8_t*, u64_t)       ;
struct __io_task* __udp_recv_from(__udp*, u8_t*, u64_t, __v4*);

#endif