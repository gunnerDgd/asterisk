#include "io_dev.h"
#include "io_sched.h"

obj_trait io_dev_trait     = {
    .on_new   = &io_dev_new  ,
    .on_clone = &io_dev_clone,
    .on_ref   = 0            ,
    .on_del   = &io_dev_del  ,
    .size     = sizeof(io_dev)
};

obj_trait* io_dev_t = &io_dev_trait;

bool_t
    io_dev_new
        (io_dev* par_dev, u32_t par_count, va_list par)                       {
            io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, void*);
            int       dev   = 0; if (par_count > 1) dev   = va_arg(par, int)  ;
            if (!sched)                        return false_t;
            if (!dev)                          return false_t;
            if (trait_of(sched) != io_sched_t) return false_t;
            struct epoll_event event =                       {
                .data.ptr = par_dev,
                .events   = EPOLLIN
            };

            if (epoll_ctl(sched->hnd, EPOLL_CTL_ADD, dev, &event)) return false_t;
            return true_t;
}

bool_t
    io_dev_clone
        (io_dev* par, io_dev* par_clone) {
            return false_t;
}

void
    io_dev_del
        (io_dev* par)                                             {
            epoll_ctl(par->sched->hnd, EPOLL_CTL_DEL, par->dev, 0);
            del      (par->sched);
}

io_stat*
    io_dev_stat
        (io_dev* par)                              {
            if (!par)                      return 0;
            if (trait_of(par) != io_dev_t) return 0;
            return &par->stat;
}