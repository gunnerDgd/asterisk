#include "lib.h"
#include "details/lib.h"

bool_t lib_init  () { return __lib_init  (); }
void   lib_deinit() {        __lib_deinit(); }