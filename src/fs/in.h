#ifndef __FS_IN_H__
#define __FS_IN_H__

#ifdef PRESET_LINUX
#include "epoll/in.h"
#endif

#ifdef PRESET_WIN32
#include "iocp/in.h"
#endif

#endif
