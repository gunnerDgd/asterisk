#ifndef NET_EPOLL_NET_H
#define NET_EPOLL_NET_H

#include <core.h>

extern obj_trait *net_t;
struct            net  {
    obj head;
};

extern struct net net;

bool_t net_new  (struct net*, u32_t, va_list);
bool_t net_clone(struct net*, struct net*)   ;
bool_t net_ref  (struct net*)                ;
void   net_del  (struct net*)                ;

#endif
