#ifndef __END_H__
#define __END_H__

#include <obj.h>

extern obj_trait* end_t;
typedef struct    end { u8_t end[128]; } end;

struct v4* end_as_v4(end*);
struct v6* end_as_v6(end*);
u16_t      end_port (end*);
bool_t     end_v4   (end*);
bool_t     end_v6   (end*);
obj_trait* end_af   (end*);

#endif