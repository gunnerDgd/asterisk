#ifndef __CORE_CORE_H__
#define __CORE_CORE_H__

#ifdef PRESET_WIN32
#include "win32/core.h"
#elif PRESET_LINUX
#include "linux/epoll/core.h"
#endif

#endif