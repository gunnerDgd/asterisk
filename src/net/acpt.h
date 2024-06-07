#ifndef __NET_ACPT_H__
#define __NET_ACPT_H__

#ifdef PRESET_WIN32
#include "iocp/acpt.h"
#elif  PRESET_LINUX
#include "epoll/acpt.h"
#endif

#endif