#ifndef IO_IOCP_VMA_H
#define IO_IOCP_VMA_H

#include <core.h>

extern obj_trait *vma_t;
typedef struct    vma  {
    obj   head;
    any_t map;
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
