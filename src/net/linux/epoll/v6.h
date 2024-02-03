#ifndef __NET_LINUX_EPOLL_V6_H__
#define __NET_LINUX_EPOLL_V6_H__

#include "../../../core.h"

#include <obj.h>
#include <str.h>

#include <arpa/inet.h>

extern obj_trait* v6_t  ;
typedef struct    v6    {
    obj             head;
    struct in6_addr v6  ;
}   v6;

v6*    v6_from_str (str*)       ;
v6*    v6_from_cstr(const char*);

bool_t v6_new   (v6*, u32_t, va_list);
bool_t v6_clone (v6*, v6*)           ;
void   v6_del   (v6*)                ;
str*   v6_as_str(v6*)                ;

#endif
