#ifndef __ACPT_H__
#define __ACPT_H__

#include "end.h"

extern obj_trait* acpt_t;
typedef struct    acpt { u8_t acpt[256]; } acpt;

bool_t acpt_open (acpt*, end*, obj_trait*);
void   acpt_close(acpt*);
fut*   acpt_fut  (acpt*);

#endif