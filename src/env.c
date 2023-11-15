#include "env.h"
#include "details/env.h"

bool_t env_init  () { return __env_init(); }
void   env_deinit() { __env_deinit(); }