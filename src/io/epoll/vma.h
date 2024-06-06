#ifndef __EPOLL_VMA_H__
#define __EPOLL_VMA_H__

#include <core.h>

struct file;

extern obj_trait *vma_t;
typedef struct    vma  {
    obj          head;
    struct file *file;
    u64_t        len;
    u64_t        off;
    any_t        ptr;
}   vma;

bool_t vma_new  (vma*, u32_t, va_list);
bool_t vma_clone(vma*, vma*)          ;
void   vma_del  (vma*)                ;

bool_t vma_sync (vma*);
any_t  vma_ptr  (vma*);
u64_t  vma_off  (vma*);
u64_t  vma_len  (vma*);

#endif
