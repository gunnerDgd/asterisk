#include "io_poll.h"
#include "io_run.h"

static bool_t
    do_new
        (io_poll* self, u32_t count, va_list arg)                   {
            io_run* run = 0; if (count > 0) run = va_arg(arg, any_t);
            int     dev = 0; if (count > 1) dev = va_arg(arg, i32_t);

            if (trait_of(run) != io_run_t) return false_t;
            if (dev <= 0)                  return false_t;

            struct epoll_event* event = &self->mask;
            event->events   = EPOLLIN | EPOLLERR;
            event->data.ptr = self              ;

            if (epoll_ctl(run->run, EPOLL_CTL_ADD, dev, event)) return false_t;
            self->poll.events = 0ull              ;
            self->run         = (io_run*) ref(run);
            self->dev         = dev               ;
            return true_t;
}

static bool_t
    do_clone
        (io_poll* self, io_poll* clone) {
            return false_t;
}

static void
    do_del
        (io_poll* self)            {
            io_run *run = self->run;
            epoll_ctl              (
                run->run     ,
                EPOLL_CTL_DEL,
                self->dev    ,
                0
            );

            del (run);
}


static obj_trait
    do_poll = make_trait (
        do_new         ,
        do_clone       ,
        null_t         ,
        do_del         ,
        sizeof(io_poll),
        null_t
);

obj_trait* io_poll_t = &do_poll;

void
    io_poll_mask_err
        (io_poll* self, bool_t mask)                {
            if (trait_of(self) != io_poll_t)  return;
            if (mask) self->mask.events |=  EPOLLERR;
            else      self->mask.events &= ~EPOLLERR;
            epoll_ctl                               (
                self->run->run,
                EPOLL_CTL_MOD,
                self->dev     ,
                &self->mask
            );
}

void
    io_poll_mask_out
        (io_poll* self, bool_t mask)                {
            if (trait_of(self) != io_poll_t)  return;
            if (mask) self->mask.events |=  EPOLLOUT;
            else      self->mask.events &= ~EPOLLOUT;
            epoll_ctl                               (
                self->run->run,
                EPOLL_CTL_MOD ,
                self->dev     ,
                &self->mask
            );
}

void
    io_poll_mask_in
        (io_poll* self, bool_t mask)               {
            if (trait_of(self) != io_poll_t) return;
            if (mask) self->mask.events |=  EPOLLIN;
            else      self->mask.events &= ~EPOLLIN;
            epoll_ctl                              (
                self->run->run,
                EPOLL_CTL_MOD ,
                self->dev     ,
                &self->mask
            );
}

bool_t
    io_poll_hang
        (io_poll* self)                                    {
            if (trait_of(self) != io_poll_t) return false_t;
            if (self->poll.events  & EPOLLHUP)             {
                self->poll.events ^= EPOLLHUP;
                return true_t;
            }
            return false_t;
}

bool_t
    io_poll_in
        (io_poll* self)                                    {
            if (trait_of(self) != io_poll_t) return false_t;
            if (self->poll.events  & EPOLLIN)              {
                self->poll.events ^= EPOLLIN;
                return true_t;
            }
            return false_t;
}

bool_t
    io_poll_out
        (io_poll* self)                                    {
            if (trait_of(self) != io_poll_t) return false_t;
            if (self->poll.events  & EPOLLOUT)             {
                self->poll.events ^= EPOLLOUT;
                return true_t;
            }
            return false_t;
}

bool_t
    io_poll_err
        (io_poll* self)                                    {
            if (trait_of(self) != io_poll_t) return false_t;
            if (self->poll.events  & EPOLLERR)             {
                self->poll.events ^= EPOLLERR;
                return true_t;
            }
            return false_t;
}