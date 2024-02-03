#include "io_res.h"
#include <errno.h>

obj_trait io_res_trait     = {
    .on_new   = &io_res_new  ,
    .on_clone = &io_res_clone,
    .on_ref   = 0            ,
    .on_del   = &io_res_del  ,
    .size     = sizeof(io_res)
};

obj_trait *io_res_t = &io_res_trait;

u64_t
    io_res_do_poll
        (io_res* par)                                      {
            if (!par)                      return fut_err  ;
            if (trait_of(par) != io_res_t) return fut_err  ;
            if (par->stat == fut_ready)    return fut_ready;
            i64_t ret = par->ops                           (
                par->dev           ,
                par->buf + par->ret,
                par->len - par->ret,
                par->arg
            );

            if (errno == EAGAIN)      goto poll_pend;
            if (errno == EINPROGRESS) goto poll_pend;
            if (ret == -1)            goto poll_err ;
            par->stat = fut_ready;
            par->ret  = ret      ;
            return par->stat;
    poll_pend:
            if (ret != -1) par->ret += ret;
            par->stat = fut_pend;
            return par->stat;
    poll_err :
            par->stat = fut_err;
            return par->stat;
}

u64_t
    io_res_do_ret
        (io_res* par)                                    {
            if (!par)                      return fut_err;
            if (trait_of(par) != io_res_t) return fut_err;
            return par->ret;
}

fut_ops io_res_fut_ops   = {
    .poll = &io_res_do_poll,
    .ret  = &io_res_do_ret
};

bool_t
    io_res_new
        (io_res* par_res, u32_t par_count, va_list par)                    {
            obj       *dev = 0; if (par_count > 0) dev = va_arg(par, obj*) ;
            io_res_ops ops = 0; if (par_count > 1) ops = va_arg(par, void*);
            void      *buf = 0; if (par_count > 2) buf = va_arg(par, void*);
            u64_t      len = 0; if (par_count > 3) len = va_arg(par, u64_t);
            void      *arg = 0; if (par_count > 4) arg = va_arg(par, void*);
            if (!dev) return false_t;
            if (!ops) return false_t;

            par_res->stat = fut_pend;
            par_res->dev  = ref(dev);
            par_res->ops  = ops     ;
            par_res->buf  = buf     ;
            par_res->len  = len     ;
            par_res->arg  = arg     ;
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

fut*
    io_res_fut
        (io_res* par)                              {
            if (!par)                      return 0;
            if (trait_of(par) != io_res_t) return 0;
            return make (fut_t) from               (
                2              ,
                &io_res_fut_ops,
                par
            );
}