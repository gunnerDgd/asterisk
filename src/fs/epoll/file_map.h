#ifndef __FS_LINUX_EPOLL_FILE_MAP_H__
#define __FS_LINUX_EPOLL_FILE_MAP_H__

#include <core.h>

struct file;

extern obj_trait *file_map_t;
struct            file_map  {
    obj          head;
    struct file *file;
    u64_t        off;
    u64_t        len;
    void*        map;
};

extern mem_ops* file_map;

bool_t file_map_new  (struct file_map*, u32_t, va_list)  ;
bool_t file_map_clone(struct file_map*, struct file_map*);
bool_t file_map_ref  (struct file_map*)                  ;
void   file_map_del  (struct file_map*)                  ;

#endif