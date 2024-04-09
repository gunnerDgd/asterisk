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
        (struct file_map* par, void* par_acq, u64_t par_size)   {
            if (trait_of(par)       != file_map_t) return null_t;
            if (trait_of(par->file) != file_t)     return null_t;
            if (par->map)                          return par->map;

            if (par_size != par->len) par->len = par_size;
            par->map =  mmap                             (
                par_acq               ,
                par->len              ,
                PROT_READ | PROT_WRITE,
                MAP_SHARED            ,
                par->file->file       ,
                par->off
            );

            return par->map;
}

void
    file_map_do_rel
        (struct file_map* par, void* par_rel, u64_t par_size) {
            if (trait_of(par)       != file_map_t) return;
            if (trait_of(par->file) != file_t)     return;
            if (par_rel != par->map)               return;
            munmap(par_rel, par_size);
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
        (struct file_map* par_map, u32_t par_count, va_list par)             {
            file* file = null_t; if (par_count > 0) file = va_arg(par, void*);
            u64_t off  = 0ull  ; if (par_count > 1) off  = va_arg(par, u64_t);
            if (trait_of(file) != file_t) return false_t;
            par_map->file = ref (file);
            par_map->off  = off;
            return true_t;
}

bool_t file_map_clone(struct file_map* par, struct file_map* par_clone) { return false_t; }
bool_t file_map_ref  (struct file_map* par)                             { return false_t; }
void   file_map_del  (struct file_map* par)                             { del(par->file); }