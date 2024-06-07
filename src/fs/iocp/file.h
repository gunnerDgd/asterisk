#ifndef __IOCP_FILE_H__
#define __IOCP_FILE_H__

#include <core.h>
#include <collections.h>

#include "../../io.h"
#include "out.h"
#include "in.h"

struct io_sched;

extern obj_trait* file_t;
typedef struct    file    {
    obj              head ;
    struct io_sched *sched;
    any_t            ioc;
    any_t            dev;
    out              out;
    in               in;
}   file;

bool_t file_new        (file*, u32_t, va_list);
bool_t file_clone      (file*, file*)         ;
void   file_del        (file*)                ;

bool_t file_open       (file*, str*)          ;
bool_t file_open_cstr  (file*, const char*)   ;
bool_t file_create     (file*, str*)          ;
bool_t file_create_cstr(file*, const char*)   ;
void   file_close      (file*)                ;

u64_t  file_seek       (file*, obj_trait*, u64_t);
u64_t  file_pos        (file*, obj_trait*)       ;
bool_t file_resize     (file*, u64_t)            ;

fut*   file_write      (file*, u8_t*, u64_t);
fut*   file_read       (file*, u8_t*, u64_t);
u64_t  file_size       (file*)              ;

#endif