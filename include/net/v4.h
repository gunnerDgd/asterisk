#ifndef __V4_H__
#define __V4_H__

#include <core.h>
#include <collections.h>

extern obj_trait* v4_t;
typedef struct    v4 { u8_t v4[64]; } v4;

v4*    make_v4_cstr(const char*);
v4*    make_v4_int (u32_t)      ;
v4*    make_v4     (str*)       ;
u32_t  v4_as_int   (v4*)        ;
str*   v4_as_str   (v4*)        ;

#endif