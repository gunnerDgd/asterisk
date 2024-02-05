#ifndef __NET_V6_H__
#define __NET_V6_H__

#ifdef PRESET_WIN32
#include "win32/v6.h"
#elif  PRESET_LINUX
#include "linux/epoll/v6.h"
#endif

#endif