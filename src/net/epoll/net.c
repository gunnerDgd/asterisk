#include "net.h"

obj_trait net_trait = make_trait (
    net_new           ,
    net_clone         ,
    net_ref           ,
    net_del           ,
    sizeof(struct net),
    null_t
);

obj_trait *net_t = &net_trait;

struct net net;

bool_t
    net_new
        (struct net* self, u32_t count, va_list arg) {
#ifndef PRESET_SUB_LIBC
            return false_t;
#endif
#ifndef PRESET_FEATURE_COLLECTIONS
            return false_t;
#endif
            return true_t;
}

bool_t net_clone(struct net* self, struct net* clone) { return false_t; }
bool_t net_ref  (struct net* self)                    { return false_t; }
void   net_del  (struct net* self)                    { return; }