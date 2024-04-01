#ifndef __NET_V4_H__
#define __NET_V4_H__

#ifdef PRESET_WIN32
#include "iocp/v4.h"
#elif  PRESET_LINUX
#include "epoll/v4.h"
#endif

#endif