#ifndef __IO_SCHED_H__
#define __IO_SCHED_H__

#ifdef PRESET_WIN32
#include "win32/io_sched.h"
#elif PRESET_LINUX
#include "epoll/io_sched.h"
#endif

#endif