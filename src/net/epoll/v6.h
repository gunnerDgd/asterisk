#ifndef NET_EPOLL_V6_H
#define NET_EPOLL_V6_H

#include <core.h>
#include <collections.h>

#include <arpa/inet.h>

extern obj_trait* v6_t  ;
typedef struct    v6    {
    obj             head;
    struct in6_addr v6  ;
}   v6;

#endif
