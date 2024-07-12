#include "v6.h"

static str*
    do_as
        (v6* self, obj_trait* trait)                 {
            if (trait_of(self) != v6_t) return null_t;
            if (trait != str_t)         return null_t;
            str *ret = make (str) from (0);

            str_prep_back(ret, 64);
            inet_ntop             (
                AF_INET6    ,
                &self->v6   ,
                str_ptr(ret),
                64
            );

            return ret;
}

static cast
    do_cast = make_cast (
        do_as ,
        null_t,
        null_t,
        null_t,
        null_t,
        null_t,
        null_t,
        null_t,
        null_t,
        null_t,
        null_t,
        null_t
);

static obj_ops
    do_ops    =          {
        .cast = &do_cast
};

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
}

static obj_trait
    do_v6 = make_trait (
        do_new    ,
        do_clone  ,
        null_t    ,
        do_del    ,
        sizeof(v6),
        &do_ops
);

obj_trait* v6_t = &do_v6;