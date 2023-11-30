#ifndef __ENV_WIN32_FILE_H__
#define __ENV_WIN32_FILE_H__

#include <obj.h>

extern obj_trait __file_trait       ;
typedef struct   __file             {
    obj                head         ;
    void              *file         ;
    void              *file_io_sched;
    struct __io_sched *io_sched     ;
}   __file;

bool_t            __file_new   (__file*, u32_t, va_list);
bool_t            __file_clone (__file*, __file*)       ;
void              __file_del   (__file*)                ;

bool_t            __file_open  (__file*, const char*)   ;
bool_t            __file_create(__file*, const char*)   ;
void              __file_close (__file*)                ;

struct __io_task* __file_read  (__file*, u8_t*, u64_t)  ;
struct __io_task* __file_write (__file*, u8_t*, u64_t)  ;

#endif