#ifndef __NET_TCP_H__
#define __NET_TCP_H__

#ifdef PRESET_WIN32
#include "win32/tcp.h"
#elif  PRESET_LINUX
#include "linux/epoll/tcp.h"
#endif

#endif