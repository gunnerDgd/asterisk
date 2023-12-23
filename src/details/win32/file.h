#ifndef __ENV_WIN32_FILE_H__
#define __ENV_WIN32_FILE_H__

#include <obj.h>

extern obj_trait* file_t;
typedef struct    file            {
    obj              head         ;
    void            *file         ;
    void            *file_io_sched;
    struct io_sched *io_sched     ;
}   file;

bool_t          file_new   (file*, u32_t, va_list);
bool_t          file_clone (file*, file*)         ;
void            file_del   (file*)                ;

bool_t          file_open  (file*, const char*)   ;
bool_t          file_create(file*, const char*)   ;
void            file_close (file*)                ;

struct io_task* file_read  (file*, u8_t*, u64_t)  ;
struct io_task* file_write (file*, u8_t*, u64_t)  ;

#endif