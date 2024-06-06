#ifndef __FS_H__
#define __FS_H__

#include "fs/file.h"
#include "fs/out.h"
#include "fs/in.h"

extern obj_trait *fs_t;
struct fs { u8_t fs[1 KB]; };

extern struct fs fs;

#endif