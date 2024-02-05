#ifndef __NET_LINUX_EPOLL_END_H__
#define __NET_LINUX_EPOLL_END_H__

#include <obj.h>
#include <arpa/inet.h>

extern obj_trait *end_t     ;
typedef struct    end       {
    obj                 head;
    u64_t               len ;
    union                   {
    sa_family_t         af  ;
    struct sockaddr_in  v4  ;
    struct sockaddr_in6 v6  ;
    struct sockaddr_in6 all ;
    };
}   end;

bool_t end_new  (end*, u32_t, va_list);
bool_t end_clone(end*, end*)          ;
void   end_del  (end*)                ;

struct v4;
struct v6;
struct v4* end_as_v4(end*);
struct v6* end_as_v6(end*);
u16_t      end_port (end*);
bool_t     end_v4   (end*);
bool_t     end_v6   (end*);
obj_trait* end_af   (end*);

#endif
