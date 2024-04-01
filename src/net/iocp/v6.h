#ifndef __NET_IOCP_V6_H__
#define __NET_IOCP_V6_H__

#include "net.h"

#include <core.h>
#include <collections.h>

extern obj_trait* v6_t  ;
typedef struct    v6    {
    obj             head;
    struct in6_addr v6  ;
}   v6;

v6*    make_v6     (str*)               ;
v6*    make_v6_cstr(const char*)        ;

bool_t v6_new      (v6*, u32_t, va_list);
bool_t v6_clone    (v6*, v6*)           ;
void   v6_del      (v6*)                ;
str*   v6_as_str   (v6*)                ;

#endif
