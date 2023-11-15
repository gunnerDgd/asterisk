#ifndef __ENV_WIN32_FILE_H__
#define __ENV_WIN32_FILE_H__

#include "io_sched.h"

extern obj_trait __file_trait      ;
typedef struct   __file            {
    obj         head               ;
    str         path               ;
    HANDLE      file, file_io_sched;
    __io_sched* io_sched           ;
}   __file;

bool_t __file_init         (__file*, u32_t, va_list);
bool_t __file_init_as_clone(__file*, __file*);
void   __file_deinit       (__file*);
str*   __file_name         (__file*);
u64_t  __file_obj_size     ()       ;

bool_t __file_open  (__file*);
bool_t __file_create(__file*);
void   __file_close (__file*);

task*  __file_read  (__file*, ptr, u64_t);
task*  __file_write (__file*, ptr, u64_t);

#endif