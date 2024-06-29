#ifndef FS_EPOLL_FILE_H
#define FS_EPOLL_FILE_H

#include <core.h>
#include <collections.h>
#include <thread.h>

#include "out.h"
#include "in.h"

extern obj_trait* file_t;
typedef struct    file  {
    obj head;
    int file;
    out out;
    in  in;
}   file;

bool_t file_open (file*, const char*);
bool_t file_new  (file*, const char*);
void   file_close(file*)             ;

u64_t  file_seek (file*, obj_trait*, u64_t);
u64_t  file_pos  (file*, obj_trait*)       ;
bool_t file_trunc(file*, u64_t)            ;

fut*   file_write(file*, u8_t*, u64_t);
fut*   file_read (file*, u8_t*, u64_t);
u64_t  file_size (file*)              ;

#endif