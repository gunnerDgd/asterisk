#ifndef __NET_TCP_H__
#define __NET_TCP_H__

#ifdef PRESET_WIN32
#include "iocp/tcp.h"
#elif  PRESET_LINUX
#include "epoll/tcp.h"
#endif

#endif