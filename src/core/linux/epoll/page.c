#include "page.h"

void* 
    page_new
        (mem* par, u64_t par_size)    {
            if (!par)         return 0;
            if (par != &page) return 0;
            return mmap               (
                0                     ,
                4 KB                  ,
                PROT_READ | PROT_WRITE,
                MAP_ANONYMOUS         ,
                -1,
                0
            );
}

void  
    page_del
        (mem* par, void* par_del)   {
            if (!par)         return;
            if (par != &page) return;
            munmap (par_del, 4 KB);
}