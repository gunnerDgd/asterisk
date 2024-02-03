#ifndef __CORE_HEAP_H__
#define __CORE_HEAP_H__

#ifdef PRESET_WIN32
#include "win32/heap.h"
#elif PRESET_LINUX
#include "linux/epoll/heap.h"
#endif

#endif