#ifndef __NET_EPOLL_V6_H__
#define __NET_EPOLL_V6_H__

#include <core.h>
#include <collections.h>

#include <arpa/inet.h>

extern obj_trait* v6_t  ;
typedef struct    v6    {
    obj             head;
    struct in6_addr v6  ;
}   v6;

v6*    make_v6_cstr(const char*);
v6*    make_v6     (str*)       ;

bool_t v6_new      (v6*, u32_t, va_list);
bool_t v6_clone    (v6*, v6*)           ;
void   v6_del      (v6*)                ;
str*   v6_as_str   (v6*)                ;

#endif
