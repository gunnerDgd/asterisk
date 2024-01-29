#ifndef __CORE_WIN32_CORE_H__
#define __CORE_WIN32_CORE_H__

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#include <mem.h>

bool_t core_new  ();
void   core_del  ();

#endif