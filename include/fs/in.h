#ifndef __IN_H__
#define __IN_H__

#include <core.h>

extern obj_trait *in_t;
typedef struct    in { u8_t in[64]; } in;

bool_t in_create_cstr(in*, const char*);
bool_t in_create     (in*, str*);

bool_t in_open_cstr  (in*, const char*);
bool_t in_open       (in*, str*);
void   in_close      (in*);

fut*   in_read       (in*, any_t, u64_t);
u64_t  in_seek       (in*, u64_t)       ;
u64_t  in_pos        (in*)              ;

#endif
