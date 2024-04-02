#ifndef __IOCP_FILE_H__
#define __IOCP_FILE_H__

#include <core.h>
#include <collections.h>
#include "../../io.h"

extern obj_trait* file_t;
typedef struct    file  {
    obj       head   ;
    io_sched *sched  ;
    void     *file_io;
    void     *file   ;
    u64_t     off    ;
}   file;

bool_t file_new        (file*, u32_t, va_list);
bool_t file_clone      (file*, file*)         ;
void   file_del        (file*)                ;

bool_t file_open       (file*, str*)          ;
bool_t file_open_cstr  (file*, const char*)   ;
bool_t file_create     (file*, str*)          ;
bool_t file_create_cstr(file*, const char*)   ;
void   file_close      (file*)                ;

fut*   file_read       (file*, u8_t*, u64_t)  ;
fut*   file_write      (file*, u8_t*, u64_t)  ;

#endif