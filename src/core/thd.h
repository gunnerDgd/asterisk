#ifndef __CORE_THD_H__
#define __CORE_THD_H__

#ifdef PRESET_WIN32
#include "win32/thd.h"
#elif PRESET_LINUX
#include "linux/epoll/thd.h"
#endif

#endif