#ifndef NET_END_H
#define NET_END_H

#include <core.h>

extern obj_trait* end_t;
typedef struct    end { u8_t end[128]; } end;

struct v4* end_v4    (end*);
struct v6* end_v6    (end*);
u16_t      end_port  (end*);
obj_trait* end_af    (end*);

#endif