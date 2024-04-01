#ifndef __NET_END_H__
#define __NET_END_H__

#ifdef PRESET_WIN32
#include "iocp/end.h"
#elif  PRESET_LINUX
#include "epoll/end.h"
#endif

#endif