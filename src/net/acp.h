#ifndef NET_ACP_H
#define NET_ACP_H

#ifdef PRESET_WIN32
#include "iocp/acp.h"
#elif  PRESET_LINUX
#include "epoll/acp.h"
#endif

#endif