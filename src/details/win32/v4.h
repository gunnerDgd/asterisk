#ifndef __ENV_WIN32_V4_H__
#define __ENV_WIN32_V4_H__

#include "env.h"
#include "io_sched.h"

extern obj_trait __v4_trait;
typedef struct   __v4      {
    obj                head;
    struct sockaddr_in v4  ;
}   __v4;

bool_t __v4_init         (__v4*, u32_t, va_list);
bool_t __v4_init_as_clone(__v4*, __v4*)         ;
void   __v4_deinit       (__v4*)                ;
u64_t  __v4_size         ()                     ;

u32_t  __v4_addr(__v4*, const char*);
u16_t  __v4_port(__v4*, u16_t)      ;

#endif