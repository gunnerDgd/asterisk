#ifndef NET_V4_H
#define NET_V4_H

#include <core.h>
#include <collections.h>

extern obj_trait* v4_t;
typedef struct    v4 { u8_t v4[64]; } v4;

u32_t v4_int(v4*);
str*  v4_str(v4*);

#endif