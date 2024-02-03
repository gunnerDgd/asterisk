#include "tcp_acpt.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

obj_trait tcp_acpt_trait     = {
	.on_new	  = &tcp_acpt_new  ,
	.on_clone = &tcp_acpt_clone,
	.on_ref   =				  0,
	.on_del	  = &tcp_acpt_del  ,
	.size	  = sizeof(tcp_acpt)
};

obj_trait* tcp_acpt_t = &tcp_acpt_trait;

i64_t
    tcp_acpt_do_run
        (tcp_acpt* par, u8_t* par_buf, u64_t par_len, tcp* par_arg)                             {
            if (!par)                                  return -1     ; struct sockaddr_in  addr ;
            if (trait_of(par)           != tcp_acpt_t) return -1     ; u64_t  len = sizeof(addr);
            if (trait_of(par_arg)       != tcp_t)      return -1     ;
            if (trait_of(&par_arg->dev) == io_dev_t)   return par_arg;
            if (!io_dev_stat(&par->dev)->in)           {
                errno = EAGAIN;
                return 0;
            }

            int  res = accept (par->tcp, &addr, &len); if (res < 0) return -1;
            if (!make_at(&par_arg->dev, io_dev_t) from (2, par->sched, res)) {
                close(res);
                return  -1;
            }

            par_arg->tcp = res;
            errno        = 0  ;
            return par_arg;
}

bool_t 
	tcp_acpt_new
		(tcp_acpt* par_acpt, u32_t par_count, va_list par)						  {
			io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
			if (!sched)					            sched = this_io_sched()       ;
			if (!sched)					       return false_t;
			if (trait_of(sched) != io_sched_t) return false_t;

			int tcp = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
			if (tcp <= 0) return false_t;
			par_acpt->sched = ref(sched);
			par_acpt->tcp   = tcp       ;
			return true_t;
}

bool_t 
	tcp_acpt_clone
		(tcp_acpt* par, tcp_acpt* par_clone) {
			return false_t;
}

void   
	tcp_acpt_del
		(tcp_acpt* par)	    {
			del (&par->tcp) ;
			del (par->sched);
}

bool_t
	tcp_acpt_conn
		(tcp_acpt* par, obj* par_addr)					        {
			if (!par_addr)					      return false_t;
			if (!par)						      return false_t;
			if (trait_of(par)      != tcp_acpt_t) return false_t;
			if (trait_of(par_addr) == v4_t)       return tcp_acpt_conn_v4(par, par_addr);
			return false_t;
}

bool_t 
	tcp_acpt_conn_v4
		(tcp_acpt* par, v4* par_addr)							{
			if (!par_addr)						  return false_t;
			if (!par)						      return false_t;
			if (trait_of(par_addr) != v4_t)		  return false_t;
			if (trait_of(par)      != tcp_acpt_t) return false_t;

			struct sockaddr_in v4 = par_addr->v4;
			if (bind  (par->tcp, &v4, sizeof(v4)))                            return false_t;
			if (listen(par->tcp, -1))			                              return false_t;
			if (!make_at(&par->dev, io_dev_t) from (2, par->sched, par->tcp)) return false_t;
			return true_t;
}

void
	tcp_acpt_close
		(tcp_acpt* par)	   {
			del(par->sched);
			del(&par->tcp) ;
}

fut*
	tcp_acpt_run
		(tcp_acpt* par)								 {
			if (!par)						 return 0;
			if (trait_of(par) != tcp_acpt_t) return 0;
            tcp    *tcp = make (tcp_t)    from (1, par->sched); if (trait_of(tcp) != tcp_t) return 0;
            io_res *ret = make (io_res_t) from (
                5              ,
                par            ,
                tcp_acpt_do_run,
                0              ,
                1              ,
                tcp
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
            if (trait_of(fut) != fut_t)    return 0; del (ret);
            return fut;
}