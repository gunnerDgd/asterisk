#ifndef NET_UDP_H
#define NET_UDP_H

#ifdef PRESET_WIN32
#include "iocp/udp.h"
#elif  PRESET_LINUX
#include "epoll/udp.h"
#endif

#endif