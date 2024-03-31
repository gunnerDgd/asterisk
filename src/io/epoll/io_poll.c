#include "io_poll.h"
#include "io_sched.h"

obj_trait io_poll_trait = make_trait (
    io_poll_new    ,
    io_poll_clone  ,
    null_t         ,
    io_poll_del    ,
    sizeof(io_poll),
    null_t
);

obj_trait* io_poll_t = &io_poll_trait;

bool_t
    io_poll_new
        (io_poll* par_dev, u32_t par_count, va_list par)                       {
            io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, void*);
            int       dev   = 0; if (par_count > 1) dev   = va_arg(par, int)  ;
            if (trait_of(sched) != io_sched_t) return false_t;
            if (dev <= 0)                      return false_t;
            struct epoll_event* event = &par_dev->mask;
            event->events   = EPOLLIN | EPOLLERR;
            event->data.ptr = par_dev           ;

            if (epoll_ctl(sched->hnd, EPOLL_CTL_ADD, dev, event)) return false_t;
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
        (io_poll* par, bool_t par_mask)                {
            if (trait_of(par) != io_poll_t)      return;
            if (par_mask) par->mask.events |=  EPOLLERR;
            else          par->mask.events &= ~EPOLLERR;
            epoll_ctl                                  (
                par->sched->hnd,
                EPOLL_CTL_MOD  ,
                par->dev       ,
                &par->mask
            );
}

void
    io_poll_mask_out
        (io_poll* par, bool_t par_mask)                {
            if (trait_of(par) != io_poll_t)      return;
            if (par_mask) par->mask.events |=  EPOLLOUT;
            else          par->mask.events &= ~EPOLLOUT;
            epoll_ctl                                  (
                par->sched->hnd,
                EPOLL_CTL_MOD  ,
                par->dev       ,
                &par->mask
            );
}

void
    io_poll_mask_in
        (io_poll* par, bool_t par_mask)               {
            if (trait_of(par) != io_poll_t)     return;
            if (par_mask) par->mask.events |=  EPOLLIN;
            else          par->mask.events &= ~EPOLLIN;
            epoll_ctl                                 (
                par->sched->hnd,
                EPOLL_CTL_MOD  ,
                par->dev       ,
                &par->mask
            );
}

bool_t
    io_poll_hang
        (io_poll* par)                                    {
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
            if (trait_of(par) != io_poll_t) return false_t;
            if (par->poll.events  & EPOLLERR)             {
                par->poll.events ^= EPOLLERR;
                return true_t;
            }
            return false_t;
}