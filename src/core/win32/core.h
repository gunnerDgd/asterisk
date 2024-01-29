#ifndef __CORE_WIN32_CORE_H__
#define __CORE_WIN32_CORE_H__

#include "ws2.h"
#include <mem.h>

bool_t core_new  ();
void   core_del  ();

int    async_main();

#endif