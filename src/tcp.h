#ifndef __TCP_H__
#define __TCP_H__

#include "base/task.h"

extern obj_trait* tcp_t;
typedef struct    tcp { u8_t tcp[256] } tcp;

task* tcp_conn (tcp*, const char*, u16_t);
task* tcp_close(tcp*);

task* tcp_send (tcp*, mem, u64_t);
task* tcp_recv (tcp*, mem, u64_t);

#endif