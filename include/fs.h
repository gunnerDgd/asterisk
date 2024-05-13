#ifndef __FS_H__
#define __FS_H__

#include "fs/file.h"
#include "fs/file_map.h"

extern obj_trait *fs_t;
struct fs { u8_t fs[1 KB]; };

extern struct fs fs;

#endif