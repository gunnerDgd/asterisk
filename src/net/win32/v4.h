#ifndef __NET_WIN32_V4_H__
#define __NET_WIN32_V4_H__

#include "../../core.h"

#include <obj.h>
#include <str.h>

extern obj_trait* v4_t     ;
typedef struct    v4       {
    obj                head;
    u64_t              size;
    struct sockaddr_in v4  ;
}   v4;

bool_t v4_new      (v4*, u32_t, va_list);
bool_t v4_clone    (v4*, v4*)           ;
void   v4_del      (v4*)                ;

u32_t  v4_as_int   (v4*)                ;
str*   v4_as_str   (v4*)                ;
void   v4_from_int (v4*, u32_t)         ;
void   v4_from_str (v4*, str*)          ;
void   v4_from_cstr(v4*, const char*)   ;
u16_t  v4_port     (v4*, u16_t)         ;

#endif