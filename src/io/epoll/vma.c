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
        (vma* self, u32_t count, va_list arg)                            {
            file *file = null_t; if (count > 0) file = va_arg(arg, any_t);
            u64_t len  = 4 KB;   if (count > 1) len  = va_arg(arg, u64_t);
            u64_t off  = 0ull;   if (count > 2) off  = va_arg(arg, u64_t);
            int   fd   =   -1;

            if (trait_of(file) == file_t) fd = file->file;
            self->ptr = mmap                             (
                null_t                ,
                len                   ,
                PROT_READ | PROT_WRITE,
                MAP_ANONYMOUS         ,
                fd                    ,
                off
            );

            if (self->ptr == (any_t)-1) return false_t;
            self->file = ref (file);
            self->len  = len;
            self->off  = off;
            return true_t;
}

bool_t
    vma_clone
        (vma* self, vma* clone) {
            return false_t;
}

void
    vma_del
        (vma* self)                      {
            munmap (self->ptr, self->len);
            del    (self->file);
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