#include "v4.h"
#include "endian.h"

#include <stdio.h>
#include <string.h>

static bool_t
    do_new
        (v4* par_v4, u32_t par_count, va_list par) {
            par_v4->v4.s_addr = 0;
            return true_t;
}

static bool_t
    do_clone
        (v4* par, v4* par_clone)   {
            par->v4 = par_clone->v4;
            return true_t; 
}

static void
    do_del
        (v4* par) {
}

static obj_trait
    do_v4 = make_trait (
        do_new    ,
        do_clone  ,
        null_t    ,
        do_del    ,
        sizeof(v4),
        null_t
);

obj_trait* v4_t = &do_v4;

u32_t 
    v4_int
        (v4* par)                               {
            if (trait_of(par) != v4_t)  return 0;
            return be32(par->v4.s_addr);
}

str*
    v4_str
        (v4* par)                                                                  {
            if (trait_of(par) != v4_t)  return null_t; str *ret = make(str) from(0);
            if (trait_of(ret) != str_t) return null_t;
            const char *buf = inet_ntoa(par->v4);

            str_push_back_cstr (ret, buf, strlen(buf));
            return ret;
}