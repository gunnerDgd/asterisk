#include "v4.h"
#include "endian.h"

#include <stdio.h>
#include <string.h>

obj_trait v4_trait = make_trait (
    v4_new    ,
    v4_clone  ,
    null_t    ,
    v4_del    ,
    sizeof(v4),
    null_t
);

obj_trait* v4_t = &v4_trait;

v4*
    make_v4_int
        (u32_t par)                                                               {
            v4 *ret = make (v4) from (0); if (trait_of(ret) != v4_t) return null_t;
            ret->v4.s_addr = be32(par);
            return ret;
}

v4*
    make_v4
        (str* par)                              {
            if (trait_of(par) != str_t) return 0;
            return make_v4_cstr(str_ptr(par));
}

v4*
    make_v4_cstr
        (const char* par)                                                         {
            if (!par)                  return null_t; v4 *ret = make (v4) from (0);
            if (trait_of(ret) != v4_t) return 0;
            ret->v4.s_addr = inet_addr(par);
            return ret;
}

bool_t 
    v4_new
        (v4* par_v4, u32_t par_count, va_list par) {
            par_v4->v4.s_addr = 0;
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
}

u32_t 
    v4_as_int
        (v4* par)                               {
            if (trait_of(par) != v4_t)  return 0;
            return be32(par->v4.s_addr);
}

str*
    v4_as_str
        (v4* par)                                                                  {
            if (trait_of(par) != v4_t)  return null_t; str *ret = make(str) from(0);
            if (trait_of(ret) != str_t) return null_t;
            const char *buf = inet_ntoa(par->v4);
            str_push_back_cstr                  (
                ret       ,
                buf       ,
                strlen(buf)
            );

            return ret;
}