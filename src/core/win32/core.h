#ifndef __CORE_WIN32_CORE_H__
#define __CORE_WIN32_CORE_H__

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

bool_t core_new  ();
void   core_del  ();
int    async_main();

#endif