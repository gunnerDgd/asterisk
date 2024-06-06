#include "io_res.h"
#include <errno.h>

obj_trait io_res_trait = make_trait (
    io_res_new    ,
    io_res_clone  ,
    null_t        ,
    io_res_del    ,
    sizeof(io_res),
    null_t
);

obj_trait *io_res_t = &io_res_trait;

bool_t
    io_res_new
        (io_res* self, u32_t count, va_list par)                       {
            obj  *dev = null_t; if (count > 0) dev = va_arg(par, obj*) ;
            void *buf = null_t; if (count > 1) buf = va_arg(par, void*);
            u64_t len = 0;      if (count > 2) len = va_arg(par, u64_t);
            any_t arg = 0;      if (count > 3) arg = va_arg(par, any_t);
            if (!dev) return false_t;

            self->stat = fut_pend;
            self->dev  = ref(dev);
            self->buf  = buf     ;
            self->len  = len     ;
            self->arg  = arg     ;
            return true_t;
}

bool_t
    io_res_clone
        (io_res* par, io_res* par_clone) {
            return false_t;
}

void
    io_res_del
        (io_res* par)     {
            del (par->dev);
}