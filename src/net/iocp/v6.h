#ifndef NET_IOCP_V6_H
#define NET_IOCP_V6_H

#include "../../io.h"
#include <collections.h>

extern obj_trait* v6_t  ;
typedef struct    v6    {
    obj             head;
    struct in6_addr v6  ;
}   v6;

#endif
