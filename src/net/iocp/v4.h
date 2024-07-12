#ifndef NET_IOCP_V4_H
#define NET_IOCP_V4_H

#include "../../io.h"
#include <collections.h>

extern obj_trait* v4_t ;
typedef struct    v4   {
    obj            head;
    struct in_addr v4  ;
}   v4;

#endif