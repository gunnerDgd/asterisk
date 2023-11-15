#ifndef __IO_SCHED_H__
#define __IO_SCHED_H__

#include "task.h"

extern obj_trait* io_sched_t;
typedef struct    io_sched { u8_t io_sched[2048]; } io_sched;

void    io_sched_run     (io_sched*)	   ;
void    io_sched_dispatch(io_sched*, task*);
#define io_sched_main(par_task, par_io_sched)                                \
    void* async_main(task*, io_sched*);                                      \
    int   main      ()                {                                      \
        io_sched *sched     = make(io_sched_t) from(0)                      ;\
        task     *main_task = make(task_t)     from(3, 0, async_main, sched);\
                                                                             \
        io_sched_dispatch(sched, main_task);                                 \
        io_sched_run     (sched)           ;                                 \
                                                                             \
        del(main_task);                                                      \
        del(sched)    ;                                                      \
    }                                                                        \
                                                                             \
    void* async_main(task* par_task, io_sched* par_io_sched)

#endif