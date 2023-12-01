#ifndef __THD_H__
#define __THD_H__

#include <obj.h>

extern obj_trait* thd_t;
typedef struct    thd { u8_t thd[2048]; } thd;

thd* thd_curr();

#endif