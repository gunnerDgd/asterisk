#ifndef __NET_UDP_H__
#define __NET_UDP_H__

#ifdef PRESET_WIN32
#include "win32/udp.h"
#elif  PRESET_LINUX
#include "linux/epoll/tcp.h"
#endif

#endif