#ifndef NET_V4_H
#define NET_V4_H

#ifdef PRESET_WIN32
#include "iocp/v4.h"
#elif  PRESET_LINUX
#include "epoll/v4.h"
#endif

#endif