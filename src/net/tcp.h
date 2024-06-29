#ifndef NET_TCP_H
#define NET_TCP_H

#ifdef PRESET_WIN32
#include "iocp/tcp.h"
#elif  PRESET_LINUX
#include "epoll/tcp.h"
#endif

#endif