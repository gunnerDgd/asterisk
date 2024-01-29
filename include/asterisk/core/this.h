#ifndef __THIS_H__
#define __THIS_H__

#include <fut.h>

void   await_all_va(u32_t, va_list)       ;
void   await_all   (u32_t, ...)           ;
void*  await       (fut*)                 ;
fut*   async       (void(*)(void*), void*);
void   yield       ()                     ;

#endif