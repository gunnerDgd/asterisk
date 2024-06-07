#include "tcp.h"

#include "../acpt.h"
#include "../tcp.h"
#include "../v6.h"
#include "../v4.h"

#include "../../../io.h"

u64_t
	do_tcp_poll
		(io_res* self)														    {
			if (trait_of(self) != io_res_t) return fut_err; tcp* tcp = self->ret;
			if (trait_of(tcp)  != tcp_t)	return fut_err;

			if (self->stat == fut_pend) io_sched_run(self->sched);
			return self->stat;
}

void*
	do_tcp_ret
		(io_res* self)						 								    {
			if (trait_of(self) != io_res_t)  return null_t; tcp* tcp = self->ret;
			if (trait_of(tcp)  != tcp_t)     return null_t;
			if (self->stat     != fut_ready) return null_t;
			return self->ret;
}

static u8_t    do_buf[128] = { 0x00, };
static u64_t   do_len      = 128;
static fut_ops do_tcp	   = make_fut_ops (
	do_tcp_poll,
	do_tcp_ret
);

bool_t
    do_tcp_open
        (struct acpt* self)														{
			if (trait_of (self) != acpt_t)                        return false_t;
            if (!make_at (&self->tcp, tcp) from (1, self->sched)) return false_t;
            if (!tcp_open(&self->tcp, end_af(self->end)))         return false_t;
            tcp  *tcp = &self->tcp;
            end  *end =  self->end;
            u64_t af  = 0;

            if (end_af(end) == v6_t) af = sizeof (struct sockaddr_in6);
            if (end_af(end) == v4_t) af = sizeof (struct sockaddr_in6);
            if (!af)                 return false_t;

            if (bind  (tcp->tcp, &self->end->all, af)) return false_t;
            if (listen(tcp->tcp, -1))                  return false_t;
            return true_t;
			
}

void
    do_tcp_close
        (struct acpt* self)						{
			if (trait_of(self) != acpt_t) return;
			tcp_close(&self->tcp);
}

fut*
    do_tcp_fut
        (struct acpt* self)																	        {
			if (trait_of(self) != acpt_t) return null_t; tcp *arg = make (tcp) from (1, self->sched);
			if (trait_of(arg)  != tcp_t)  return null_t; tcp *dev = &self->tcp;
			if (trait_of(dev)  != tcp_t)  return null_t;
			io_res *res = make(io_res) from(1, self->sched);
			fut    *ret = null_t;

			if (!tcp_open(arg, end_af(self->end))) goto err;
			if (trait_of (res) != io_res_t)        goto err;
			bool_t err = AcceptEx						   (
				dev->tcp ,
				arg->tcp ,
				do_buf	 ,
				0		 ,
				64		 ,
				64		 ,
				&do_len  ,
				&res->res
			);

			ret = make(fut) from (2, &do_tcp, res);
			if (!err && WSAGetLastError() != ERROR_IO_PENDING) {
				res->stat = fut_err			 ;
				res->ret  = WSAGetLastError();

				del   (res);
				del   (arg);
				return ret;
			}
			
			res->ret = arg;
			del   (res);
			return ret ;
	err:	del     (arg);
			del		(ret);
			del		(res);
			return null_t;
}