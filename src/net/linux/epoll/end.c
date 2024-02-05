#include "end.h"
#include "v4.h"
#include "v6.h"

obj_trait end_trait     = {
    .on_new   = &end_new  ,
    .on_clone = &end_clone,
    .on_ref   = 0         ,
    .on_del   = &end_del  ,
    .size     = sizeof(end)
};

obj_trait* end_t = &end_trait;

bool_t
    end_new
        (end* par_end, u32_t par_count, va_list par)                    {
            obj*  addr = 0; if (par_count > 0) addr = va_arg(par, obj*) ;
            u16_t port = 0; if (par_count > 1) port = va_arg(par, u32_t);
            if (par_count == 0)                    {
                par_end->af  = AF_UNSPEC           ;
                par_end->len = sizeof(par_end->all);
                return true_t ;
            }
            if (trait_of(addr) == v4_t)                         {
                par_end->v4.sin_family = AF_INET                ;
                par_end->v4.sin_addr   = ((v4*)addr)->v4        ;
                par_end->v4.sin_port   = be16(port)             ;
                par_end->len           = sizeof(par_end->v4);
                return true_t;
            }

            if (trait_of(addr) == v6_t)                          {
                par_end->v6.sin6_family = AF_INET6           ;
                par_end->v6.sin6_port   = be16(port)         ;
                par_end->v6.sin6_addr   = ((v6*)addr)->v6    ;
                par_end->len            = sizeof(par_end->v6);
                return true_t;
            }

            return false_t;
}

bool_t
    end_clone
        (end* par, end* par_clone)   {
            par->all = par_clone->all;
            par->len = par_clone->len;
            return true_t;
}

void
    end_del
        (end* par) {
            return;
}

bool_t
    end_v4
        (end* par)                                    {
            if (!par)                   return false_t;
            if (trait_of(par) != end_t) return false_t;
            return par->af == AF_INET;
}

bool_t
    end_v6
        (end* par)                                    {
            if (!par)                   return false_t;
            if (trait_of(par) != end_t) return false_t;
            return par->af == AF_INET6;
}

struct v4*
    end_as_v4
        (end* par)                              {
            if (!par)                   return 0;
            if (trait_of(par) != end_t) return 0;
            if (!end_v4 (par))          return 0;

            v4* ret = make (v4_t) from (0);
            if (!ret)                  return 0;
            if (trait_of(ret) != v4_t) return 0;
            ret->v4 = par->v4.sin_addr;
            return ret;
}

struct v6*
    end_as_v6
        (end* par)                              {
            if (!par)                   return 0;
            if (trait_of(par) != end_t) return 0;
            if (!end_v6 (par))          return 0;

            v6* ret = make (v6_t) from (0);
            if (!ret)                  return 0;
            if (trait_of(ret) != v6_t) return 0;
            ret->v6 = par->v6.sin6_addr;
            return ret;
}

u16_t
    end_port
        (end* par)                              {
            if (!par)                   return 0;
            if (trait_of(par) != end_t) return 0;
            switch (par->len)                                       {
                case sizeof(par->v4): return be16(par->v4.sin_port) ;
                case sizeof(par->v6): return be16(par->v6.sin6_port);
                default                 : return 0;
            }
}

obj_trait*
    end_af
        (end* par)                              {
            if (!par)                   return 0;
            if (trait_of(par) != end_t) return 0;
            switch (par->af)                    {
                case AF_INET : return v4_t;
                case AF_INET6: return v6_t;
                default      : return 0   ;
            }
}