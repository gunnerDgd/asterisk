#ifndef __NET_LINUX_EPOLL_V4_H__
#define __NET_LINUX_EPOLL_V4_H__

#include "../../../core.h"

#include <obj.h>
#include <str.h>

#include <arpa/inet.h>

extern obj_trait* v4_t ;
typedef struct    v4   {
    obj            head;
    struct in_addr v4  ;
}   v4;

v4*    v4_from_int (u32_t)      ;
v4*    v4_from_str (str*)       ;
v4*    v4_from_cstr(const char*);

bool_t v4_new      (v4*, u32_t, va_list);
bool_t v4_clone    (v4*, v4*)           ;
void   v4_del      (v4*)                ;
u32_t  v4_as_int   (v4*)                ;
str*   v4_as_str   (v4*)                ;

#endif