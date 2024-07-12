#ifndef NET_EPOLL_V4_H
#define NET_EPOLL_V4_H

#include <core.h>
#include <collections.h>

#include <arpa/inet.h>

extern obj_trait* v4_t ;
typedef struct    v4   {
    obj            head;
    struct in_addr v4  ;
}   v4;

#endif