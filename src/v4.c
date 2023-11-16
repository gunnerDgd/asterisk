#include "v4.h"
#include "details/v4.h"

obj_trait* v4_t = &__v4_trait;

u32_t 
    v4_addr
        (v4* par, const char* par_v4)          {
            if (!par)                  return 0;
            if (trait_of(par) != v4_t) return 0;

            return __v4_addr(par, par_v4);
}

u16_t 
    v4_port
        (v4* par, u16_t par_port) {
            if (!par)                  return 0;
            if (trait_of(par) != v4_t) return 0;

            return __v4_port(par, par_port);
}