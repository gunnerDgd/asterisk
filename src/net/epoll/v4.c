#include "v4.h"
#include "endian.h"

#include <stdio.h>
#include <string.h>

static i32_t do_as_i32(v4* self) { if (trait_of(self) != v4_t) return -1; return be32(self->v4.s_addr); }
static u32_t do_as_u32(v4* self) { if (trait_of(self) != v4_t) return -1; return be32(self->v4.s_addr); }

static str*
    do_as
        (v4* self, obj_trait* trait)                 {
            if (trait_of(self) != v4_t) return null_t;
            if (trait != str_t)         return null_t;
            const char *buf = inet_ntoa(self->v4);
            str        *ret = make (str) from (0);

            str_push_back(ret, buf, strlen(buf));
            return ret;
}

static cast
    do_cast = make_cast (
        do_as    ,
        null_t   ,
        null_t   ,
        null_t   ,
        null_t   ,
        do_as_i32,
        do_as_u32,
        null_t   ,
        null_t   ,
        null_t   ,
        null_t   ,
        null_t
);

static obj_ops
    do_ops    =          {
        .cast = &do_cast
};

static bool_t
    do_new
        (v4* self, u32_t count, va_list arg) {
            self->v4.s_addr = 0;
            return true_t;
}

static bool_t
    do_clone
        (v4* self, v4* clone)   {
            self->v4 = clone->v4;
            return true_t; 
}

static void
    do_del
        (v4* self) {
}

static obj_trait
    do_v4 = make_trait (
        do_new    ,
        do_clone  ,
        null_t    ,
        do_del    ,
        sizeof(v4),
        &do_ops
);

obj_trait* v4_t = &do_v4;