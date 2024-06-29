#ifndef NET_IOCP_NET_H
#define NET_IOCP_NET_H

#include "../../io.h"

extern LPFN_CONNECTEX    ConnectEx       ;
extern GUID			     ConnectExGuid   ;
extern LPFN_DISCONNECTEX DisconnectEx    ;
extern GUID				 DisconnectExGuid;

extern obj_trait *net_t;
struct            net  {
    obj     head    ;
    WSADATA ws2_info;
    SOCKET  ws2     ;
};

extern struct net net;

bool_t net_new  (struct net*, u32_t, va_list);
bool_t net_clone(struct net*, struct net*)   ;
bool_t net_ref  (struct net*)                ;
void   net_del  (struct net*)                ;

#endif