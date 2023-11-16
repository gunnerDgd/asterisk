#include "v4.h"

obj_trait __v4_trait                  = {
    .init          = &__v4_init         ,
    .init_as_clone = &__v4_init_as_clone,
    .init_as_ref   =                   0,
    .deinit        = &__v4_deinit       ,
    .size          = &__v4_size         ,
    .name          =                   0
};

bool_t 
    __v4_init
        (__v4* par_v4, u32_t par_count, va_list par) {
            if(par_count == 0)                       {
                par_v4->v4.sin_addr.s_addr = 0       ;
                par_v4->v4.sin_port        = 0       ;
                par_v4->v4.sin_family      = AF_INET ;

                return true_t;
            }

            if(par_count == 2)                                                  {
                par_v4->v4.sin_addr.s_addr = inet_addr(va_arg(par, const char*));
                par_v4->v4.sin_port        = htons    (va_arg(par, u16_t))      ;
                par_v4->v4.sin_family      = AF_INET                            ;

                return true_t;
            }

            return false_t;
}

bool_t 
    __v4_init_as_clone
        (__v4* par, __v4* par_clone) {
            par->v4 = par_clone->v4;
            return true_t;
}

void
    __v4_deinit
        (__v4* par) {
            return;
}

u64_t
    __v4_size()            {
        return sizeof(__v4);
}

u32_t  
    __v4_addr
        (__v4* par, const char* par_v4) {
            if(!par_v4) 
                return par->v4.sin_addr.s_addr;
            
                   par->v4.sin_addr.s_addr = inet_addr(par_v4);
            return par->v4.sin_addr.s_addr;
}

u16_t
    __v4_port
        (__v4* par, u16_t par_port)               {
            if(par_port != 0)
                par->v4.sin_port = htons(par_port);

            return par->v4.sin_port;
}