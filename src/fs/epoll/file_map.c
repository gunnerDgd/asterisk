#include "file_map.h"
#include "file.h"

#include <sys/mman.h>

struct file_map*
    file_map_do_new
        (u32_t par_count, va_list par)           {
            return (struct file_map*) obj_new_va (
                null_t    ,
                file_map_t,
                par_count ,
                par
            );
}

void
    file_map_do_del
        (struct file_map* par)                      {
            if  (trait_of(par) != file_map_t) return;
            del (par);
}

void*
    file_map_do_acq
        (struct file_map* self, void* hint, u64_t len)        {
            if (trait_of(self) != file_map_t) return    null_t;
            if (self->map)                    return self->map;
            u64_t off = self->off;
            u64_t dev = -1;

            if (trait_of(self->dev) == file_t) dev = self->dev->file;
            self->map =  mmap                                       (
                hint                      ,
                len                       ,
                PROT_READ     | PROT_WRITE,
                MAP_ANONYMOUS |MAP_SHARED ,
                dev                       ,
                off
            );

            self->len  = len;
            return self->map;
}

void
    file_map_do_rel
        (struct file_map* self, void* rel, u64_t len) {
            if (trait_of(self) != file_map_t) return;
            if (self->map != rel)             return;
            if (self->len != len)             return;
            munmap(rel, len);
}

mem_ops file_map_do = make_mem_ops (
        file_map_do_acq,
        file_map_do_rel,
        file_map_do_new,
        file_map_do_del
);

mem_ops*  file_map       = &file_map_do;
obj_trait file_map_trait = make_trait  (
    file_map_new           ,
    file_map_clone         ,
    file_map_ref           ,
    file_map_del           ,
    sizeof(struct file_map),
    null_t
);

obj_trait *file_map_t = &file_map_trait;

bool_t
    file_map_new
        (struct file_map* self, u32_t count, va_list arg)              {
            file* dev = null_t; if (count > 0) dev = va_arg(arg, void*);
            u64_t off = 0ull  ; if (count > 1) off = va_arg(arg, u64_t);

            if (trait_of(dev) != file_t) dev = null_t;
            self->dev = (file*) ref (dev);
            self->off  = off;

            return true_t;
}

bool_t file_map_clone(struct file_map* self, struct file_map* clone) { return false_t; }
bool_t file_map_ref  (struct file_map* self)                         { return false_t; }
void   file_map_del  (struct file_map* self)                         { del(self->map); }