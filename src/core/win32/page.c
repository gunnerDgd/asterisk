#include "page.h"

void* 
    page_new
        (mem* par, u64_t par_size)    {
            if (!par)         return 0;
            if (par != &page) return 0;
            return VirtualAlloc       (
                0            ,
                par_size     ,
                MEM_COMMIT   ,
                PAGE_READWRITE
            );
}

void  
    page_del
        (mem* par, void* par_del)                {
            if (!par)                      return;
            if (par != &page)              return;
            VirtualFree(par_del, 0, MEM_RELEASE) ;
            VirtualFree(par_del, 0, MEM_DECOMMIT);
}