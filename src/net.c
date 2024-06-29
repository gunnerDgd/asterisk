#include "net.h"

acp*
    new_acp
        (end* end, obj_trait *pro)        {
            acp *ret = make (acp) from (0);

            if (trait_of(ret) != acp_t)   goto err;
            if (!acp_open(ret, end, pro)) goto err;
            return ret;
    err:    del     (ret);
            return null_t;
}

tcp*
    new_tcp
        (obj_trait* af)                           {
            return make (tcp) from (2, null_t, af);
}


udp*
    new_udp
        (obj_trait* af)                           {
            return make (udp) from (2, null_t, af);
}

v4*
    new_v4
        (const char* str)               {
            v4 *ret = make (v4) from (0);

            if (trait_of(ret) != v4_t) goto err;
            if (!str)                  goto err;
            inet_pton(AF_INET, str, &ret->v4);
            return ret;
    err:    del     (ret);
            return null_t;
}

v6*
    new_v6
        (const char* str)               {
            v6 *ret = make (v6) from (0);

            if (trait_of(ret) != v6_t) goto err;
            if (!str)                  goto err;
            inet_pton(AF_INET6, str, &ret->v6);
            return ret;
    err:    del     (ret);
            return null_t;
}