#ifndef __FILE_H__
#define __FILE_H__

#include "sched.h"
#include "io_task.h"

extern obj_trait* file_t;
typedef struct    file { u8_t file[64]; } file;

bool_t  file_open  (file*, const char*) ;
bool_t  file_create(file*, const char*) ;
void    file_close (file*)              ;

io_task file_read  (file*, u8_t*, u64_t);
io_task file_write (file*, u8_t*, u64_t);

#endif