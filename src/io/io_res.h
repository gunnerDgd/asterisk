#ifndef __IO_RES_H__
#define __IO_RES_H__

#ifdef PRESET_WIN32
#include "win32/io_res.h"
#endif

#ifdef PRESET_LINUX
#include "epoll/io_res.h"
#endif

#endif