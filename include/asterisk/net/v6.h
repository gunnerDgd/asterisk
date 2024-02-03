#ifndef __V6_H__
#define __V6_H__

#include <obj.h>
#include <str.h>

extern obj_trait* v6_t;
typedef struct    v6 { u8_t v6[64]; } v6;

v6*    v6_from_str (str*)       ;
v6*    v6_from_cstr(const char*);
str*   v6_as_str   (v6*)        ;

#endif