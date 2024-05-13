#ifndef __FS_FILE_H__
#define __FS_FILE_H__

#ifdef PRESET_WIN32
#include "iocp/file_map.h"
#endif

#ifdef PRESET_LINUX
#include "epoll/file_map.h"
#endif

#endif