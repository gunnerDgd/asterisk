#ifndef __CORE_PAGE_H__
#define __CORE_PAGE_H__

#ifdef PRESET_WIN32
#include "win32/page.h"
#elif PRESET_LINUX
#include "linux/epoll/page.h"
#endif

#endif