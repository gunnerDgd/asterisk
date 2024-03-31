#ifndef __NET_EPOLL_V4_H__
#define __NET_EPOLL_V4_H__

#include <core.h>
#include <collections.h>

#include <arpa/inet.h>

extern obj_trait* v4_t ;
typedef struct    v4   {
    obj            head;
    struct in_addr v4  ;
}   v4;

v4*    make_v4_cstr(const char*);
v4*    make_v4_int (u32_t)      ;
v4*    make_v4     (str*)       ;

bool_t v4_new      (v4*, u32_t, va_list);
bool_t v4_clone    (v4*, v4*)           ;
void   v4_del      (v4*)                ;
u32_t  v4_as_int   (v4*)                ;
str*   v4_as_str   (v4*)                ;

#endif