#include "vma.h"
#include <Windows.h>

#include "../../fs.h"

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
        (vma* self, u32_t count, va_list arg)                         {
            obj*  dev = null_t; if (count > 0) dev = va_arg(arg, any_t);
            u64_t len = 4 KB  ; if (count > 1) len = va_arg(arg, u64_t);
            u64_t off = 0ull  ; if (count > 2) off = va_arg(arg, u64_t);

            HANDLE hnd = INVALID_HANDLE_VALUE;
            DWORD  per = 0;

            if (trait_of(dev) == file_t) { hnd = ((out*)dev)->dev; per = PAGE_READWRITE; }
            if (trait_of(dev) == out_t)  { hnd = ((out*)dev)->dev; per = PAGE_READWRITE; }
            if (trait_of(dev) == in_t)   { hnd = ((out*)dev)->dev; per = PAGE_READONLY ; }
            if (!per) per = GENERIC_READ | GENERIC_WRITE;

            self->map = CreateFileMapping (
                hnd            ,
                null_t         ,
                per            , 
                shr(len, 32)   , 
                len & mask (32), 
                null_t
            );

            if (!self->map) return false_t;
            self->dev = ref (dev);
            self->ptr = null_t;
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
        (vma* self)                                  {
            if (self->ptr) UnmapViewOfFile(self->ptr);
            CloseHandle (self->map);
            del         (self->dev);
            
}

bool_t 
    vma_sync 
        (vma* self)                                    {
            if (trait_of(self) != vma_t) return false_t;
            if (!self->ptr)              vma_ptr (self);
            FlushViewOfFile(self->ptr, self->len);
            return true_t;
}

any_t  
    vma_ptr  
        (vma* self)                                      {
            if (trait_of(self) != vma_t) return    null_t;
            if (self->ptr)               return self->ptr;
            u64_t per = 0;

            if (trait_of(self->dev) == file_t) per = FILE_MAP_WRITE | FILE_MAP_READ;
            if (trait_of(self->dev) == out_t)  per = FILE_MAP_WRITE;
            if (trait_of(self->dev) == in_t)   per = FILE_MAP_READ;
            if (!per)                          per = FILE_MAP_WRITE | FILE_MAP_READ;
            self->ptr = MapViewOfFile                                              (
                self->map            ,
                per                  ,
                shl   (self->off, 32),
                self->off & mask (32),
                self->len
            );

            return self->ptr;
}

u64_t  
    vma_off  
        (vma* self)                               {
            if (trait_of(self) != vma_t) return -1;
            return self->off;
}

u64_t  
    vma_len  
        (vma* self)                              {
            if (trait_of(self) != vma_t) return 0;
            return self->len;
}