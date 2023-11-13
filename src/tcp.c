#include "tcp.h"
#include "env/tcp.h"

obj_trait* tcp_t = &__tcp_trait;

task* 
	tcp_conn
		(tcp* par, const char* par_v4, u16_t par_port) {
			if(trait_of(par) != tcp_t)
				return false_t;

			return __tcp_conn(par, par_v4, par_port);
}

task* 
	tcp_close
		(tcp* par) {
			if(trait_of(par) != tcp_t)
				return false_t;

			return __tcp_close(par);
}

task* 
	tcp_send
		(tcp* par, mem par_buf, u64_t par_len) {
			if(trait_of(par) != tcp_t)
				return false_t;

			return __tcp_send(par, par_buf, par_len);
}

task* 
	tcp_recv
		(tcp* par, mem par_buf, u64_t par_len) {
			if(trait_of(par) != tcp_t)
				return false_t;

			return __tcp_recv(par, par_buf, par_len);
}