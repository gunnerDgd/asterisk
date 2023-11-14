#ifndef __ENV_WIN32_ENV_H__
#define __ENV_WIN32_ENV_H__

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#include "../../base/type.h"

extern LPFN_CONNECTEX    ConnectEx       ;
extern GUID			     ConnectExGuid   ;
extern LPFN_DISCONNECTEX DisconnectEx    ;
extern GUID				 DisconnectExGuid;

bool_t __env_init  ();
void   __env_deinit();

#endif