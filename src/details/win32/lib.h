#ifndef __DETAILS_WIN32_LIB_H__
#define __DETAILS_WIN32_LIB_H__

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#include <mem.h>

extern LPFN_CONNECTEX    ConnectEx       ;
extern GUID			     ConnectExGuid   ;
extern LPFN_DISCONNECTEX DisconnectEx    ;
extern GUID				 DisconnectExGuid;
extern WSADATA			 lib_info        ;
extern SOCKET			 lib             ;

bool_t __lib_init  ();
void   __lib_deinit();

#endif