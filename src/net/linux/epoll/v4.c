#include "v4.h"

#include "endian.h"
#include <stdio.h>

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
            par_v4->size               = 0      ;
            par_v4->v4.sin_family      = AF_INET;
            par_v4->v4.sin_addr.s_addr = 0      ;
            par_v4->v4.sin_port        = 0      ;
            return true_t;
}

bool_t 
    v4_clone
        (v4* par, v4* par_clone)       {
            par->size = par_clone->size;
            par->v4   = par_clone->v4  ;
            return true_t; 
}

void
    v4_del(v4* par) { 
        return; 
}

u32_t 
    v4_as_int
        (v4* par)                               {
            if (!par)                   return 0;
            if (trait_of(par) != v4_t)  return 0;
            return be32(par->v4.sin_addr.s_addr);
}

str*
    v4_as_str
        (v4* par)                               { 
            if (!par)                   return 0;
            if (trait_of(par) != v4_t)  return 0;

            str        *ret = make(str_t) from(0)        ;
            const char *buf = inet_ntoa(par->v4.sin_addr);
            if (!ret)                   return 0;
            if (trait_of(ret) != str_t) return 0;

            str_push_back_cstr(ret, buf, strlen(buf));
            return ret;
}

void   
    v4_from_int 
        (v4* par, u32_t par_v4)                   {
            if (!par)                       return;
            if (trait_of(par) != v4_t)      return;
            par->v4.sin_addr.s_addr = be32(par_v4);
}

void   
    v4_from_str 
        (v4* par, str* par_v4)                       {
            if (!par_v4)                       return;
            if (!par)                          return;
            if (trait_of(par_v4) != str_t)     return;
            if (trait_of(par)    != v4_t)      return;
            return v4_from_cstr(par, str_ptr(par_v4));
}

void   
    v4_from_cstr
        (v4* par, const char* par_v4)                  {
            if (!par_v4)                         return;
            if (!par)                            return;
            if (trait_of(par) != v4_t)           return;
            par->v4.sin_addr.s_addr = inet_addr(par_v4);
}


u16_t 
    v4_port
        (v4* par, u16_t par_port)               {
            if (!par)                  return -1;
            if (trait_of(par) != v4_t) return -1;
            if (!par_port)             return be16(par->v4.sin_port);

            par->v4.sin_port = be16(par_port);
            return  par_port;
}