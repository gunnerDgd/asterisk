#ifndef __FILE_H__
#define __FILE_H__

#include "sched.h"
#include "task.h"

extern obj_trait* file_t;
typedef struct    file { u8_t file[256]; } file;

bool_t file_open  (file*);
bool_t file_create(file*);
void   file_close (file*);

task*  file_read  (file*, ptr, u64_t);
task*  file_write (file*, ptr, u64_t);

#endif