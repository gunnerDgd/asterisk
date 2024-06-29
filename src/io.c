#include "io.h"

thd_local struct io io;

bool_t do_new  (struct io* self, u32_t count, va_list arg) { return make_at (&self->run, io_run) from (0); }
bool_t do_clone(struct io* self, struct io* clone)         { return  false_t; }
bool_t do_ref  (struct io* self)                           { return  false_t; }
void   do_del  (struct io* self)                           { del(&self->run); }


static obj_trait
    do_io = make_trait   (
        do_new           ,
        do_clone         ,
        do_ref           ,
        do_del           ,
        sizeof(struct io),
    null_t
);

obj_trait *io_t = &do_io;

io_run*
    this_io_run()     {
        return &io.run;
}