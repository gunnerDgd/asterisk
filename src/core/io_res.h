#ifndef __CORE_IO_RES_H__
#define __CORE_IO_RES_H__

#ifdef PRESET_WIN32
#include "win32/io_res.h"
#elif PRESET_LINUX
#include "linux/epoll/io_res.h"
#endif

#endif