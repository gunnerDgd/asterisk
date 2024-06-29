#ifndef NET_V6_H
#define NET_V6_H

#include <core.h>
#include <collections.h>

extern obj_trait* v6_t;
typedef struct    v6 { u8_t v6[64]; } v6;

str* v6_str(v6*);

#endif