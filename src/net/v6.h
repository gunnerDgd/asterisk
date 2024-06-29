#ifndef NET_V6_H
#define NET_V6_H

#ifdef PRESET_WIN32
#include "iocp/v6.h"
#elif  PRESET_LINUX
#include "epoll/v6.h"
#endif

#endif