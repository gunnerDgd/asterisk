#include "heap.h"
#include <stdlib.h>

void* 
    heap_new
        (mem* par, u64_t par_size)    {
            if (!par)         return 0;
            if (par != &heap) return 0;
            return malloc(par_size);
}

void  
    heap_del
        (mem* par, void* par_del)   {
            if (!par)         return;
            if (par != &heap) return;
            free(par_del);
}

void* 
    heap_align_new
        (mem* par, u64_t par_size)              {
            if (!par)                   return 0;
            if (par != &heap_align)     return 0;
            return aligned_alloc(par_size, 16);
}

void  
    heap_align_del
        (mem* par, void* par_del)          {
            if  (!par)               return;
            if  (par != &heap_align) return;
            free(par_del);
}

mem heap    =          {
    .on_new = &heap_new,
    .on_del = &heap_del
};

mem heap_align =             {
    .on_new = &heap_align_new,
    .on_del = &heap_align_del
};