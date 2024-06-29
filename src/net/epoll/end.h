#ifndef NET_EPOLL_END_H
#define NET_EPOLL_END_H

#include <core.h>
#include <arpa/inet.h>

struct v4;
struct v6;

extern obj_trait *end_t     ;
typedef struct    end       {
    obj                 head;
    u64_t               len;
    union                  {
    struct sockaddr_in6 all;
    struct sockaddr_in6 v6;
    struct sockaddr_in  v4;
    sa_family_t         af;
    };
}   end;

struct v4* end_v4    (end*);
struct v6* end_v6    (end*);
u16_t      end_port  (end*);
obj_trait* end_af    (end*);

#endif
