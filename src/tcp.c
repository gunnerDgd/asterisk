#include "tcp.h"
#include "v4.h"

#include "details/tcp.h"

obj_trait* tcp_t = &__tcp_trait;

task* 
    tcp_conn
        (tcp* par, obj* par_addr)  {
            if (!par)      return 0;
            if (!par_addr) return 0;

            if (trait_of(par)      != tcp_t) return 0;
            if (trait_of(par_addr) == v4_t)  return __tcp_conn(par, par_addr);

            return 0;
}

task* 
    tcp_close
        (tcp* par)                              {
            if (!par)                   return 0;
            if (trait_of(par) != tcp_t) return 0;

            return __tcp_close(par);
}

task* 
    tcp_send
        (tcp* par, ptr par_buf, u64_t par_len)  {
            if (!par)                   return 0;
            if (trait_of(par) != tcp_t) return 0;

            return __tcp_send(par, par_buf, par_len); 
}

task* 
    tcp_recv
        (tcp* par, ptr par_buf, u64_t par_len)  {
            if (!par)                   return 0;
            if (trait_of(par) != tcp_t) return 0;

            return __tcp_recv(par, par_buf, par_len);
}