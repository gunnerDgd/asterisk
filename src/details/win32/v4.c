#include "v4.h"
#include <endian.h>

obj_trait v4_trait     = {
    .on_new   = &v4_new  ,
    .on_clone = &v4_clone,
    .on_ref   =         0,
    .on_del   = &v4_del  ,
    .size     = sizeof(v4)
};

obj_trait* v4_t = &v4_trait;

bool_t 
    v4_new
        (v4* par_v4, u32_t par_count, va_list par) {
            par_v4->v4.sin_family = AF_INET;
            const char* addr = 0;
            u16_t       port = 0;

            switch (par_count)                          {
                case 0 : addr = 0                       ; 
                         port = 0                       ;

                         par_v4->v4.sin_addr.s_addr = inet_addr("0.0.0.0");
                         par_v4->v4.sin_port        = be16     (port)     ;
                         break                          ;
                case 1 : addr = va_arg(par, const char*);
                         port = 0                       ;

                         par_v4->v4.sin_addr.s_addr = inet_addr(addr);
                         par_v4->v4.sin_port        = be16     (port);
                         break                          ;
                case 2 : addr = va_arg(par, const char*);
                         port = va_arg(par, u16_t)      ;
                         
                         par_v4->v4.sin_addr.s_addr = inet_addr(addr);
                         par_v4->v4.sin_port        = be16     (port);
                         
                         break;
                default: return false_t;
            }

            return true_t;
}

bool_t 
    v4_clone
        (v4* par, v4* par_clone)   {
            par->v4 = par_clone->v4;
            return true_t; 
}

void
    v4_del(v4* par) { 
        return; 
}

u32_t 
    v4_as_int
        (v4* par)                                {
            if (!par)                  return -1u;
            if (trait_of(par) != v4_t) return -1u;
                        
            return be32(par->v4.sin_addr.s_addr); 
}

str*
    v4_as_str
        (v4* par)                                 { 
            if (!par)                  return NULL;
            if (trait_of(par) != v4_t) return NULL;

            const char *ret_str = inet_ntoa(par->v4.sin_addr);
            str        *ret     = make(str_t) from (0)       ; if (!ret) return false_t;

            str_push_back_cstr(ret, ret_str, strlen(ret));
            return ret;
}

u16_t 
    v4_port
        (v4* par)                               {
            if (!par)                  return -1;
            if (trait_of(par) != v4_t) return -1;

            return be16(par->v4.sin_port);
}