#include "v4.h"
#include <endian.h>

obj_trait __v4_trait     = {
    .on_new   = &__v4_new  ,
    .on_clone = &__v4_clone,
    .on_ref   =           0,
    .on_del   = &__v4_del  ,
    .size     = sizeof(__v4)
};

bool_t 
    __v4_new
        (__v4* par_v4, u32_t par_count, va_list par) {
            par_v4->host.sin_family = AF_INET;
            if(par_count == 2)               {
                par_v4->host.sin_addr.s_addr = inet_addr(va_arg(par, const char*));
                par_v4->host.sin_port        = be16     (va_arg(par, u16_t))      ;
            }

            return true_t;
}

bool_t 
    __v4_clone
        (__v4* par, __v4* par_clone)   { 
            par->host = par_clone->host;
            par->peer = par_clone->peer;

            mem_copy(par->host_pad, par_clone->host_pad, 16);
            mem_copy(par->peer_pad, par_clone->peer_pad, 16);

            return true_t; 
}

void
    __v4_del(__v4* par) { 
        return; 
}

u32_t __v4_addr(__v4* par) { return be32(par->host.sin_addr.s_addr); }
u16_t __v4_port(__v4* par) { return be16(par->host.sin_port)       ; }