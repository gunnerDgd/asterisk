#ifndef NET_END_H
#define NET_END_H

#ifdef PRESET_WIN32
#include "iocp/end.h"
#elif  PRESET_LINUX
#include "epoll/end.h"
#endif

#endif