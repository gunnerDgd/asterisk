#ifndef __IO_VMA_H__
#define __IO_VMA_H__

#include <core.h>

extern obj_trait *vma_t;
typedef struct    vma { u8_t vma[128]; } vma;

bool_t vma_sync (vma*);
any_t  vma_ptr  (vma*);
u64_t  vma_off  (vma*);
u64_t  vma_len  (vma*);

#endif
