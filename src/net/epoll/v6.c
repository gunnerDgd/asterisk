#include "v6.h"
#include "endian.h"

#include <stdio.h>
#include <string.h>

obj_trait v6_trait = make_trait (
    v6_new    ,
    v6_clone  ,
    null_t    ,
    v6_del    ,
    sizeof(v6),
    null_t
);

obj_trait* v6_t = &v6_trait;

v6*
    make_v6
        (str* par)                              {
            if (trait_of(par) != str_t) return 0;
            return make_v6_cstr(str_ptr(par));
}

v6*
    make_v6_cstr
        (const char* par)                                                         {
            if (!par)                  return null_t; v6 *ret = make (v6) from (0);
            if (trait_of(ret) != v6_t) return null_t;
            inet_pton(AF_INET6, par, &ret->v6);
            return ret;
}

bool_t
    v6_new
        (v6* par_v4, u32_t par_count, va_list par) {
            return true_t;
}

bool_t
    v6_clone
        (v6* par, v6* par_clone)   {
            par->v6 = par_clone->v6;
            return true_t;
}

void
    v6_del
        (v6* par) {
}

str*
    v6_as_str
        (v6* par)                                                                  {
            if (trait_of(par) != v6_t)  return null_t; str *ret = make(str) from(0);
            if (trait_of(ret) != str_t) return null_t; str_prep_back(ret, 64);
            inet_ntop                                (
                AF_INET6    ,
                &par->v6    ,
                str_ptr(ret),
                64
            );

            return ret;
}