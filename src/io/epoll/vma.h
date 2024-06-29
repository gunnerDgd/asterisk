#ifndef EPOLL_VMA_H
#define EPOLL_VMA_H

#include <core.h>

struct file;

extern obj_trait *vma_t;
typedef struct    vma  {
    obj   head;
    obj*  dev;
    u64_t len;
    u64_t off;
    any_t ptr;
}   vma;

bool_t vma_sync (vma*);
any_t  vma_ptr  (vma*);
u64_t  vma_off  (vma*);
u64_t  vma_len  (vma*);

#endif
