#ifndef __V4_H__
#define __V4_H__

#include "obj.h"

extern obj_trait* v4_t;
typedef struct    v4 { u8_t v4[64]; } v4;

u32_t v4_addr(v4*);
u16_t v4_port(v4*);

#endif