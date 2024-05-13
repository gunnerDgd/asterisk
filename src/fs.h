#ifndef __FS_H__
#define __FS_H__

#include "fs/file.h"
#include "fs/file_map.h"

extern obj_trait *fs_t;
struct            fs  {
    obj head;
};

extern struct fs fs;

bool_t fs_new  (struct fs*, u32_t, va_list);
bool_t fs_clone(struct fs*, struct fs*)    ;
bool_t fs_ref  (struct fs*)                ;
void   fs_del  (struct fs*)                ;

#endif