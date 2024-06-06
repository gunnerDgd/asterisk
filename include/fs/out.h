#ifndef __OUT_H__
#define __OUT_H__

#include <core.h>

extern obj_trait *out_t;
typedef struct    out { u8_t out[128]; } out;

bool_t out_create_cstr(out*, const char*);
bool_t out_create     (out*, str*);

bool_t out_open_cstr  (out*, const char*);
bool_t out_open       (out*, str*);
void   out_close      (out*);

fut*   out_write      (out*, any_t, u64_t);
u64_t  out_seek       (out*, u64_t)       ;
u64_t  out_pos        (out*)              ;

#endif
