#include "end.h"

#include "v4.h"
#include "v6.h"

static bool_t
    do_new
        (end* self, u32_t count, va_list arg)                       {
            obj*  addr = 0; if (count > 0) addr = va_arg(arg, any_t);
            u16_t port = 0; if (count > 1) port = va_arg(arg, u32_t);
            if (!addr)                       {
                self->af  = AF_UNSPEC        ;
                self->len = sizeof(self->all);
                return true_t ;
            }

            if (trait_of(addr) == v4_t) goto do_v4;
            if (trait_of(addr) == v6_t) goto do_v6;
            return false_t;
    do_v4:  self->v4.sin_addr   = ((v4*)addr)->v4;
            self->v4.sin_port   = be16(port);
            self->v4.sin_family = AF_INET;
            self->len = sizeof(self->v4);
            return true_t;
    do_v6:  self->v6.sin6_addr   = ((v6*)addr)->v6;
            self->v6.sin6_port   = be16(port);
            self->v6.sin6_family = AF_INET6;
            self->len = sizeof(self->v6);
            return true_t;
}

static bool_t
    do_clone
        (end* self, end* clone)   {
            self->all = clone->all;
            self->len = clone->len;
            return true_t;
}

static void
    do_del
        (end* self) {
            return;
}

static obj_trait 
    do_obj = make_trait (
        do_new     ,
        do_clone   ,
        null_t     ,
        do_del     ,
        sizeof(end),
        null_t
);

obj_trait* end_t = &do_obj;

obj_trait* 
    end_af
        (end* self)                                   {
            if (trait_of(self) != end_t) return null_t;
            switch (self->af)         {
            case AF_INET : return v4_t;
            case AF_INET6: return v6_t;
            default      : return 0   ;
            }
}

struct v4*
    end_v4
        (end* self)                                   {
            if (trait_of(self) != end_t) return null_t;
            if (self->af != AF_INET)     return null_t;
            v4* ret = make (v4) from (0);

            if (trait_of(ret) != v4_t) return null_t;
            ret->v4 = self->v4.sin_addr;
            return ret;
}

struct v6*
    end_v6
        (end* self)                                   {
            if (trait_of(self) != end_t) return null_t;
            if (self->af != AF_INET6)    return null_t;
            v6* ret = make (v6) from (0);

            if (trait_of(ret) != v6_t) return null_t;
            ret->v6 = self->v6.sin6_addr;
            return ret;
}

u16_t
    end_port
        (end* self)                              {
            if (trait_of(self) != end_t) return 0;
            switch (self->len)                                        {
                case sizeof(self->v4): return be16(self->v4.sin_port) ;
                case sizeof(self->v6): return be16(self->v6.sin6_port);
                default             : return 0;
            }
}