#ifndef __V6_H__
#define __V6_H__

#include <core.h>
#include <collections.h>

extern obj_trait* v6_t;
typedef struct    v6 { u8_t v6[64]; } v6;

v6*  make_v6     (str*)       ;
v6*  make_v6_cstr(const char*);
str* v6_as_str   (v6*)        ;

#endif