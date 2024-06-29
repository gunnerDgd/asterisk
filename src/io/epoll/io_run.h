#ifndef EPOLL_IO_RUN_H
#define EPOLL_IO_RUN_H

#include <core.h>
#include <sys/epoll.h>

extern obj_trait* io_run_t;
typedef struct    io_run   {
	obj                head;
	struct epoll_event res[128];
	u64_t              num;
	int                run;
}	io_run;

u64_t io_run_flush(io_run*);
fut*  io_run_fut  (io_run*);

#endif