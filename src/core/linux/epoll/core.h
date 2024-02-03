#ifndef __CORE_LINUX_EPOLL_CORE_H__
#define __CORE_LINUX_EPOLL_CORE_H__

#include <sys/epoll.h>
#include <sys/mman.h>
#include <pthread.h>

#include "mem.h"

bool_t core_new  ();
void   core_del  ();

#endif