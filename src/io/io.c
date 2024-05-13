#include "io.h"

obj_trait io_trait = make_trait (
    io_new           ,
    io_clone         ,
    io_ref           ,
    io_del           ,
    sizeof(struct io),
    null_t
);

obj_trait *io_t = &io_trait;

bool_t io_new  (struct io* self, u32_t count, va_list arg) { return make_at (&self->sched, io_sched) from (0); }
bool_t io_clone(struct io* self, struct io* clone)         { return    false_t; }
bool_t io_ref  (struct io* self)                           { return    false_t; }
void   io_del  (struct io* self)                           { del(&self->sched); }

io_sched*
    this_io_sched()     {
        return &io.sched;
}