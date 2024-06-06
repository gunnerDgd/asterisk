#ifndef __FS_OUT_H__
#define __FS_OUT_H__

#ifdef PRESET_LINUX
#include "epoll/out.h"
#endif

#ifdef PRESET_WIN32
#include "iocp/out.h"
#endif

#endif
