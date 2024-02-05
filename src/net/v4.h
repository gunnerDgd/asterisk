#ifndef __NET_V4_H__
#define __NET_V4_H__

#ifdef PRESET_WIN32
#include "win32/v4.h"
#elif  PRESET_LINUX
#include "linux/epoll/v4.h"
#endif

#endif