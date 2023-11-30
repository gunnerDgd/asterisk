#ifndef __ENV_WIN32_V4_H__
#define __ENV_WIN32_V4_H__

#include "lib.h"
#include <obj.h>

extern obj_trait __v4_trait;
typedef struct   __v4      {
    obj                head;
    u64_t              size;
    struct sockaddr_in host; u8_t host_pad[16];
    struct sockaddr_in peer; u8_t peer_pad[16];
}   __v4;

bool_t __v4_new  (__v4*, u32_t, va_list);
bool_t __v4_clone(__v4*, __v4*)         ;
void   __v4_del  (__v4*)                ;
u32_t  __v4_addr (__v4*)                ;
u16_t  __v4_port (__v4*)                ;

#endif