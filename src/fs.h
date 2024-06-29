#ifndef FS_H
#define FS_H

#include "fs/file.h"
#include "fs/out.h"
#include "fs/in.h"

extern obj_trait *fs_t;
struct            fs  {
    obj head;
};

extern struct fs fs;

file* new_file(const char*);
out*  new_out (const char*);
in*   new_in  (const char*);

#endif