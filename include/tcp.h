#ifndef __TCP_H__
#define __TCP_H__

#include "task.h"
#include "io_task.h"

extern obj_trait* tcp_t;
typedef struct    tcp { u8_t tcp[256]; } tcp;

io_task tcp_conn (tcp*, obj*)        ;
void    tcp_close(tcp*)              ;
io_task tcp_send (tcp*, u8_t*, u64_t);
io_task tcp_recv (tcp*, u8_t*, u64_t);

#endif