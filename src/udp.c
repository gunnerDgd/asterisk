#include "udp.h"
#include "details/udp.h"

obj_trait* udp_t = &__udp_trait;

bool_t 
    udp_conn 
        (udp* par, obj* par_addr)        {
            if (!par)      return false_t;
            if (!par_addr) return false_t;

            if (trait_of(par)      != udp_t) return false_t;
            if (trait_of(par_addr) == v4_t)  return __udp_conn(par, par_addr);

            return false_t;
}

void 
    udp_close
        (udp* par)                            {
            if (!par)                   return;
            if (trait_of(par) != udp_t) return;

            __udp_close(par);
}

task
    udp_send
        (udp* par, u8_t* par_ptr, u64_t par_len) {
            if (!par)                   return 0;
            if (trait_of(par) != udp_t) return 0;

            return __udp_send(par, par_ptr, par_len);
}

task
    udp_send_to  
        (udp* par, u8_t* par_ptr, u64_t par_len, obj* par_addr) {
            if (!par)                        return 0;
            if (!par_addr)                   return 0;
            if (trait_of(par)      != udp_t) return 0;
            if (trait_of(par_addr) == v4_t)  return __udp_send_to(par, par_ptr, par_len, par_addr);
            return 0;
}

task 
    udp_recv
        (udp* par, u8_t* par_ptr, u64_t par_len) {
            if (!par)                   return 0;
            if (trait_of(par) != udp_t) return 0;

            return __udp_recv(par, par_ptr, par_len);
}

task 
    udp_recv_from
        (udp* par, u8_t* par_ptr, u64_t par_len, obj* par_addr) {
            if (!par)                        return 0;
            if (!par_addr)                   return 0;
            if (trait_of(par)      != udp_t) return 0;
            if (trait_of(par_addr) == v4_t)  return __udp_recv_from(par, par_ptr, par_len, par_addr);
            return 0;
}