#include "vma.h"
#include "../../fs.h"

#include <sys/mman.h>

obj_trait vma_trait = make_trait (
    vma_new    ,
    vma_clone  ,
    null_t     ,
    vma_del    ,
    sizeof(vma),
    null_t
);

obj_trait *vma_t = &vma_trait;

bool_t
    vma_new
        (vma* self, u32_t count, va_list arg)                          {
            obj  *dev = null_t; if (count > 0) dev = va_arg(arg, any_t);
            u64_t len = 4 KB;   if (count > 1) len = va_arg(arg, u64_t);
            u64_t off = 0ull;   if (count > 2) off = va_arg(arg, u64_t);

            int flag = MAP_SHARED;
            int per  =  PROT_READ | PROT_WRITE;
            int fd   = -1;

            if (trait_of(dev) == file_t) { per = PROT_READ | PROT_WRITE; fd = ((file*)dev)->file; }
            if (trait_of(dev) == out_t)  { per = PROT_WRITE;             fd = ((out*) dev)->out ; }
            if (trait_of(dev) == in_t)   { per = PROT_READ ;             fd = ((in*)  dev)->in  ; }
            if (fd == -1) flag |= MAP_ANONYMOUS;
            self->ptr = mmap                   (
                null_t,
                len   ,
                per   ,
                flag  ,
                fd    ,
                off
            );

            if (self->ptr == (any_t)-1) return false_t;
            self->dev = ref (dev);
            self->len = len;
            self->off = off;
            return true_t;
}

bool_t
    vma_clone
        (vma* self, vma* clone) {
            return false_t;
}

void
    vma_del
        (vma* self)                              {
            msync (self->ptr, self->len, MS_SYNC);
            munmap(self->ptr, self->len);
            del   (self->dev);
}

bool_t
    vma_sync
        (vma* self)                                    {
            if (trait_of(self) != vma_t) return false_t;
            msync(self->ptr, self->len, MS_SYNC);
            return true_t;
}

any_t vma_ptr(vma* self) { if (trait_of(self) != vma_t) return null_t; return self->ptr; }
u64_t vma_len(vma* self) { if (trait_of(self) != vma_t) return      0; return self->len; }
u64_t vma_off(vma* self) { if (trait_of(self) != vma_t) return      0; return self->off; }