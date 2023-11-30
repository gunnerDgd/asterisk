#include "thd.h"
#include "details/thd.h"
#include "details/curr.h"

obj_trait* thd_t = &__thd_trait;

thd* thd_curr() { return curr_thd; }