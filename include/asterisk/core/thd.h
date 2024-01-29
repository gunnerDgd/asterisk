#ifndef __THD_H__
#define __THD_H__

#include <fut.h>

extern obj_trait* thd_t;
typedef struct    thd { u8_t thd[2048]; } thd;

void thd_wait(thd*);
fut* thd_fut (thd*);

#endif