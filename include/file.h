#ifndef __FILE_H__
#define __FILE_H__

#include <fut.h>
#include <str.h>

extern obj_trait* file_t;
typedef struct    file { u8_t file[64]; } file;

bool_t file_open       (file*, str*)        ;
bool_t file_open_cstr  (file*, const char*) ;
bool_t file_create     (file*, str*)        ;
bool_t file_create_cstr(file*, const char*) ;
void   file_close      (file*)              ;

fut*   file_read       (file*, u8_t*, u64_t);
fut*   file_write      (file*, u8_t*, u64_t);

#endif