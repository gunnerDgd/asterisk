#ifndef __CORE_THIS_H__
#define __CORE_THIS_H__

#ifdef PRESET_WIN32
#include "win32/this.h"
#elif PRESET_LINUX
#include "linux/epoll/this.h"
#endif

#endif