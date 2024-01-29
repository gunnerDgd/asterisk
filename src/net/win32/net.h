#ifndef __NET_WIN32_NET_H__
#define __NET_WIN32_NET_H__

#include "../../core.h"

extern LPFN_CONNECTEX    ConnectEx       ;
extern GUID			     ConnectExGuid   ;
extern LPFN_DISCONNECTEX DisconnectEx    ;
extern GUID				 DisconnectExGuid;
extern WSADATA			 ws2_info        ;
extern SOCKET			 ws2             ;

bool_t net_new();
void   net_del();

#endif