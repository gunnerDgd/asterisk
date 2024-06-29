#ifndef __IO_SCHED_H__
#define __IO_SCHED_H__

#ifdef PRESET_WIN32
#include "iocp/io_sched.h"
#elif PRESET_LINUX
#include "epoll/io_run.h"
#endif

#endif