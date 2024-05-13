#ifndef __IO_ALL_H__
#define __IO_ALL_H__

#include "io/io_res.h"
#include "io/io_sched.h"
#include "io/io_poll.h"

#include <thread.h>


extern obj_trait *io_t;
struct            io  {
    obj      head ;
    io_sched sched;
};

bool_t io_new  (struct io*, u32_t, va_list);
bool_t io_clone(struct io*, struct io*)    ;
bool_t io_ref  (struct io*)                ;
void   io_del  (struct io*)                ;

io_sched* this_io_sched();

extern thd_local struct io io;

#endif