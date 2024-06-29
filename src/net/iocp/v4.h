#ifndef NET_IOCP_V4_H
#define NET_IOCP_V4_H

#include "../../io.h"
#include <collections.h>

extern obj_trait* v4_t ;
typedef struct    v4   {
    obj            head;
    struct in_addr v4  ;
}   v4;

u32_t  v4_int(v4*);
str*   v4_str(v4*);

#endif