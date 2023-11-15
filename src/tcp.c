#include "tcp.h"
#include "details/tcp.h"

obj_trait* tcp_t = &__tcp_trait;

task* tcp_conn (tcp* par, const char* par_v4, u16_t par_port) { return (trait_of(par) == tcp_t) ? __tcp_conn (par, par_v4, par_port) : 0; }
task* tcp_close(tcp* par)									  { return (trait_of(par) == tcp_t) ? __tcp_close(par) : 0; }

task* tcp_send (tcp* par, ptr par_buf, u64_t par_len) { return (trait_of(par) == tcp_t) ? __tcp_send(par, par_buf, par_len) : 0; }
task* tcp_recv (tcp* par, ptr par_buf, u64_t par_len) { return (trait_of(par) == tcp_t) ? __tcp_recv(par, par_buf, par_len) : 0; }