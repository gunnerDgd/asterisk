#ifndef __V4_H__
#define __V4_H__

#include <obj.h>
#include <str.h>

extern obj_trait* v4_t;
typedef struct    v4 { u8_t v4[64]; } v4;

v4*    v4_from_int (u32_t)      ;
v4*    v4_from_str (str*)       ;
v4*    v4_from_cstr(const char*);
u32_t  v4_as_int   (v4*)        ;
str*   v4_as_str   (v4*)        ;

#endif