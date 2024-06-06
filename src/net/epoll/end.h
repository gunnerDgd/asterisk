#ifndef __NET_EPOLL_END_H__
#define __NET_EPOLL_END_H__

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

bool_t     end_new  (end*, u32_t, va_list);
bool_t     end_clone(end*, end*)          ;
void       end_del  (end*)                ;

struct v4* end_as_v4(end*);
struct v6* end_as_v6(end*);
u16_t      end_port (end*);
obj_trait* end_af   (end*);

#endif
