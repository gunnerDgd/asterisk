#include "tcp_acpt.h"
#include "details/tcp_acpt.h"

obj_trait* tcp_acpt_t = &__tcp_acpt_trait;

bool_t 
	tcp_acpt_conn
		(tcp_acpt* par, obj* par_addr)					   {
			if (!par)						 return false_t;
			if (!par_addr)				     return false_t;
			if (trait_of(par) != tcp_acpt_t) return false_t;

			if (trait_of(par_addr) == v4_t)  return __tcp_acpt_conn(par, par_addr);
			return false_t;
}

void   
	tcp_acpt_close
		(tcp_acpt* par) {
			if(!par)						return;
			if(trait_of(par) != tcp_acpt_t) return;

			__tcp_acpt_close(par);
}

task* 
	tcp_acpt_run
		(tcp_acpt* par) {
			if(!par)						return 0;
			if(trait_of(par) != tcp_acpt_t) return 0;

			return __tcp_acpt_run(par);
}