#ifndef __UDP_H__
#define __UDP_H__

#include "v4.h"
#include "task.h"
#include "sched.h"

obj_trait*     udp_t;
typedef struct udp { u8_t udp[128]; } udp;

bool_t udp_conn (udp*, obj*);
void   udp_close(udp*)      ;

task   udp_send     (udp*, u8_t*, u64_t)      ;
task   udp_send_to  (udp*, u8_t*, u64_t, obj*);
task   udp_recv     (udp*, u8_t*, u64_t)      ;
task   udp_recv_from(udp*, u8_t*, u64_t, obj*);

#endif