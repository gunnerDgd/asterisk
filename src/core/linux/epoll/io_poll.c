#include "io_poll.h"
#include "io_sched.h"

obj_trait io_poll_trait     = {
    .on_new   = &io_poll_new  ,
    .on_clone = &io_poll_clone,
    .on_ref   = 0             ,
    .on_del   = &io_poll_del  ,
    .size     = sizeof(io_poll)
};

obj_trait* io_poll_t = &io_poll_trait;

bool_t
    io_poll_new
        (io_poll* par_dev, u32_t par_count, va_list par)                       {
            io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, void*);
            int       dev   = 0; if (par_count > 1) dev   = va_arg(par, int)  ;
            if (!sched)                        return false_t;
            if (!dev)                          return false_t;
            if (trait_of(sched) != io_sched_t) return false_t;
            par_dev->mask.events   = EPOLLIN | EPOLLERR;
            par_dev->mask.data.ptr = par_dev           ;

            if (epoll_ctl(sched->hnd, EPOLL_CTL_ADD, dev, &par_dev->mask)) return false_t;
            par_dev->poll.events = 0         ;
            par_dev->sched       = ref(sched);
            par_dev->dev         = dev       ;
            return true_t;
}

bool_t
    io_poll_clone
        (io_poll* par, io_poll* par_clone) {
            return false_t;
}

void
    io_poll_del
        (io_poll* par)                                             {
            epoll_ctl(par->sched->hnd, EPOLL_CTL_DEL, par->dev, 0);
            del      (par->sched);
}

void
    io_poll_mask_err
        (io_poll* par, bool_t par_mask)           {
            if (!par)                       return;
            if (trait_of(par) != io_poll_t) return;
            switch (par_mask)                                     {
                case true_t : par->mask.events |=  EPOLLERR; break;
                case false_t: par->mask.events &= ~EPOLLERR; break;
                default     : return;
            }

            epoll_ctl(par->sched->hnd, EPOLL_CTL_MOD, par->dev, &par->mask);
}

void
    io_poll_mask_out
        (io_poll* par, bool_t par_mask)           {
            if (!par)                       return;
            if (trait_of(par) != io_poll_t) return;
            switch (par_mask)                                     {
                case true_t : par->mask.events |=  EPOLLOUT; break;
                case false_t: par->mask.events &= ~EPOLLOUT; break;
                default     : return;
            }

            epoll_ctl(par->sched->hnd, EPOLL_CTL_MOD, par->dev, &par->mask);
}

void
    io_poll_mask_in
        (io_poll* par, bool_t par_mask)           {
            if (!par)                       return;
            if (trait_of(par) != io_poll_t) return;
            switch (par_mask)                                    {
                case true_t : par->mask.events |=  EPOLLIN; break;
                case false_t: par->mask.events &= ~EPOLLIN; break;
                default     : return;
            }

            epoll_ctl(par->sched->hnd, EPOLL_CTL_MOD, par->dev, &par->mask);
}

bool_t
    io_poll_hang
        (io_poll* par)                                    {
            if (!par)                       return false_t;
            if (trait_of(par) != io_poll_t) return false_t;
            if (par->poll.events  & EPOLLHUP)             {
                par->poll.events ^= EPOLLHUP;
                return true_t;
            }
            return false_t;
}

bool_t
    io_poll_in
        (io_poll* par)                                    {
            if (!par)                       return false_t;
            if (trait_of(par) != io_poll_t) return false_t;
            if (par->poll.events  & EPOLLIN)              {
                par->poll.events ^= EPOLLIN;
                return true_t;
            }
            return false_t;
}

bool_t
    io_poll_out
        (io_poll* par)                                    {
            if (!par)                       return false_t;
            if (trait_of(par) != io_poll_t) return false_t;
            if (par->poll.events  & EPOLLOUT)             {
                par->poll.events ^= EPOLLOUT;
                return true_t;
            }
            return false_t;
}

bool_t
    io_poll_err
        (io_poll* par)                                    {
            if (!par)                       return false_t;
            if (trait_of(par) != io_poll_t) return false_t;
            if (par->poll.events  & EPOLLERR)             {
                par->poll.events ^= EPOLLERR;
                return true_t;
            }
            return false_t;
}