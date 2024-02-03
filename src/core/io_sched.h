#ifndef __CORE_IO_SCHED_H__
#define __CORE_IO_SCHED_H__

#ifdef PRESET_WIN32
#include "win32/io_sched.h"
#elif PRESET_LINUX
#include "linux/epoll/io_sched.h"
#include "linux/epoll/io_dev.h"
#endif

#endif