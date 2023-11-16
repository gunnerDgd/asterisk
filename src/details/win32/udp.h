#ifndef __ENV_WIN32_UDP_H__
#define __ENV_WIN32_UDP_H__

#include "io_sched.h"
#include "v4.h"

extern obj_trait __udp_trait;
typedef struct   __udp      {
    obj         head        ;
    SOCKET      udp         ;
    HANDLE      udp_iocp    ;
    __io_sched* io_sched    ;
}   __udp;

bool_t __udp_init         (__udp*, u32_t, va_list);
bool_t __udp_init_as_clone(__udp*, __udp*)        ;
void   __udp_deinit       (__udp*)                ;
u64_t  __udp_size         ()                      ;

bool_t __udp_conn (__udp*, __v4*);
void   __udp_close(__udp*);

task*  __udp_send     (__udp*, ptr, u64_t)       ;
task*  __udp_send_to  (__udp*, ptr, u64_t, __v4*);
task*  __udp_recv     (__udp*, ptr, u64_t)       ;
task*  __udp_recv_from(__udp*, ptr, u64_t, __v4*);

#endif