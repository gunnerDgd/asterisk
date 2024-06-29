#ifndef NET_NET_H
#define NET_NET_H

#ifdef PRESET_WIN32
#include "iocp/net.h"
#elif  PRESET_LINUX
#include "epoll/net.h"
#endif

#endif