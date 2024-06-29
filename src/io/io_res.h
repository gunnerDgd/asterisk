#ifndef IO_RES_H
#define IO_RES_H

#ifdef PRESET_WIN32
#include "iocp/io_res.h"
#endif

#ifdef PRESET_LINUX
#include "epoll/io_res.h"
#endif

#endif