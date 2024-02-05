#ifndef __NET_END_H__
#define __NET_END_H__

#ifdef PRESET_WIN32
#include "win32/end.h"
#elif  PRESET_LINUX
#include "linux/epoll/end.h"
#endif

#endif