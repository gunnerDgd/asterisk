#include "v6.h"
#include "endian.h"

#include <stdio.h>
#include <string.h>

obj_trait v6_trait         = {
        .on_new   = &v6_new  ,
        .on_clone = &v6_clone,
        .on_ref   =         0,
        .on_del   = &v6_del  ,
        .size     = sizeof(v6)
};

obj_trait* v6_t = &v6_trait;

v6*
    v6_from_str
        (str* par)                              {
            if (!par)                   return 0;
            if (trait_of(par) != str_t) return 0;
            return v6_from_cstr(str_ptr(par));
}

v6*
    v6_from_cstr
        (const char* par)                      {
            v6  *ret     = make (v6_t) from (0);
            if (!ret)                  return 0;
            if (!par)                  return 0;
            if (trait_of(ret) != v6_t) return 0;

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
            return;
}

str*
    v6_as_str
        (v6* par)                                                               {
            if (trait_of(par) != v6_t)  return 0; str *ret = make(str_t) from(0);
            if (trait_of(ret) != str_t) return 0; str_rsv_back(ret, 64);
            inet_ntop(AF_INET6, &par->v6, str_ptr(ret), 64);
            return ret;
}