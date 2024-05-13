#ifndef __NET_NET_H__
#define __NET_NET_H__

#ifdef PRESET_WIN32
#include "iocp/net.h"
#elif  PRESET_LINUX
#include "epoll/net.h"
#endif

#endif