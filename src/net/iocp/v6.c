#include "v6.h"

static bool_t
    do_new
        (v6* self, u32_t count, va_list arg) {
            return true_t;
}

static bool_t
    do_clone
        (v6* self, v6* clone)   {
            self->v6 = clone->v6;
            return true_t;
}

static void
    do_del
        (v6* par) {
            return;
}

static obj_trait 
    do_obj = make_trait (
        do_new    ,
        do_clone  ,
        null_t    ,
        do_del    ,
        sizeof(v6),
        null_t
);

obj_trait* v6_t = &do_obj;

str*
    v6_str
        (v6* self)                                                                     {
            if (trait_of (self) != v6_t)  return null_t; str *ret = make (str) from (0);
            if (trait_of (ret)  != str_t) return null_t;
            str_prep_back(ret, 64);

            inet_ntop(AF_INET6, &self->v6, str_ptr(ret), 64);
            return ret;
}