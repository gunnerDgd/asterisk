#ifndef __CORE_WIN32_WS2_H__
#define __CORE_WIN32_WS2_H__

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#include <obj.h>

extern LPFN_CONNECTEX    ConnectEx       ;
extern GUID			     ConnectExGuid   ;
extern LPFN_DISCONNECTEX DisconnectEx    ;
extern GUID				 DisconnectExGuid;
extern WSADATA			 ws2_info        ;
extern SOCKET			 ws2             ;

bool_t ws2_new();
void   ws2_del();

#endif