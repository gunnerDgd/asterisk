#ifndef __IO_TASK_H__
#define __IO_TASK_H__

#include <obj.h>

typedef void* io_task;
void*         io_task_wait(io_task);

#endif