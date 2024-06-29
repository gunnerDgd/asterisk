#include "v4.h"

static bool_t 
    do_new
        (v4* self, u32_t count, va_list arg) {
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
            return;
}

static obj_trait 
    do_obj = make_trait (
        do_new    ,
        do_clone  ,
        null_t    ,
        do_del    ,
        sizeof(v4),
        null_t
);

obj_trait* v4_t = &do_obj;

u32_t 
    v4_int
        (v4* self)                               {
            if (trait_of(self) != v4_t)  return 0;
            return be32(self->v4.s_addr);
}

str*
    v4_str
        (v4* self)                                   {
            if (trait_of(self) != v4_t) return null_t;
            const char* buf = inet_ntoa(self->v4);
            str        *ret = make (str) from(0);

            str_push_back_cstr(ret, buf, strlen(buf));
            return ret;
}