#include "v4.h"
#include "endian.h"

#include <stdio.h>
#include <string.h>

obj_trait v4_trait     = {
    .on_new   = &v4_new  ,
    .on_clone = &v4_clone,
    .on_ref   =         0,
    .on_del   = &v4_del  ,
    .size     = sizeof(v4)
};

obj_trait* v4_t = &v4_trait;

v4*
    v4_from_int
        (u32_t par)                            {
            v4  *ret     = make (v4_t) from (0);
            if (!ret)                  return 0;
            if (trait_of(ret) != v4_t) return 0;
            ret->v4.s_addr = be32(par);
            return ret;
}

v4*
    v4_from_str
        (str* par)                              {
            if (!par)                   return 0;
            if (trait_of(par) != str_t) return 0;
            return v4_from_cstr(str_ptr(par));
}

v4*
    v4_from_cstr
        (const char* par)                      {
            v4  *ret     = make (v4_t) from (0);
            if (!ret)                  return 0;
            if (!par)                  return 0;
            if (trait_of(ret) != v4_t) return 0;
            ret->v4.s_addr = inet_addr(par);
            return ret;
}

bool_t 
    v4_new
        (v4* par_v4, u32_t par_count, va_list par) {
            return true_t;
}

bool_t 
    v4_clone
        (v4* par, v4* par_clone)   {
            par->v4 = par_clone->v4;
            return true_t; 
}

void
    v4_del
        (v4* par) {
            return;
}

u32_t 
    v4_as_int
        (v4* par)                               {
            if (!par)                   return 0;
            if (trait_of(par) != v4_t)  return 0;
            return be32(par->v4.s_addr);
}

str*
    v4_as_str
        (v4* par)                               { 
            if (!par)                   return 0;
            if (trait_of(par) != v4_t)  return 0;

            str        *ret = make(str_t) from(0);
            const char *buf = inet_ntoa(par->v4) ;
            if (!ret)                   return 0;
            if (trait_of(ret) != str_t) return 0;

            str_push_back_cstr(ret, buf, strlen(buf));
            return ret;
}