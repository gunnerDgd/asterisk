#include "tcp.h"
#include "v4.h"
#include "v6.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

obj_trait tcp_trait	= make_trait (
    tcp_new    ,
    tcp_clone  ,
    null_t     ,
    tcp_del    ,
    sizeof(tcp),
    null_t
);

obj_trait* tcp_t = &tcp_trait;

u64_t
    tcp_conn_do_poll
        (io_res* self)                                                                  {
            if (trait_of(self) != io_res_t) return fut_err; tcp* poll = (tcp*) self->dev;
            if (trait_of(poll) != tcp_t)    return fut_err;
            if (io_poll_hang(&poll->poll))  return fut_err;
            if (io_poll_err (&poll->poll))  return fut_err;

            if (!io_poll_out(&poll->poll)) io_sched_run(poll->sched);
            if (!io_poll_out(&poll->poll)) return fut_pend;
            io_poll_mask_out(&poll->poll, false_t);
            return fut_ready;
}

void*
    tcp_conn_do_ret
        (io_res* par)                                                        {
            if (trait_of(par) != io_res_t) return null_t; tcp* tcp = par->dev;
            if (trait_of(tcp) != tcp_t)    return null_t;
            return tcp;
}

u64_t
    tcp_recv_do_poll
        (io_res* par)                                                         {
            if (trait_of(par) != io_res_t) return fut_err; tcp* tcp = par->dev;
            if (trait_of(tcp) != tcp_t)    return fut_err;
            if (io_poll_hang(&tcp->poll))  return fut_err;
            if (io_poll_err (&tcp->poll))  return fut_err;

            if (!io_poll_in(&tcp->poll)) io_sched_run(tcp->sched);
            if (!io_poll_in(&tcp->poll)) return fut_pend;
            i64_t ret = recv                            (
                tcp->tcp,
                par->buf,
                par->len,
                tcp->flag
            );

            if (ret == -1) return fut_err ; par->ret += ret;
            if (ret ==  0) return fut_pend;
            return fut_ready;
}

u64_t
    tcp_recv_do_ret
        (io_res* par)                                                   {
            if (trait_of(par) != io_res_t) return 0; tcp* tcp = par->dev;
            if (trait_of(tcp) != tcp_t)    return 0;
            return par->ret;
}

i64_t
    tcp_send_do_poll
        (io_res* par)                                                         {
            if (trait_of(par) != io_res_t) return fut_err; tcp* tcp = par->dev;
            if (trait_of(tcp) != tcp_t)    return fut_err;
            if (io_poll_hang(&tcp->poll))  return fut_err;
            if (io_poll_err (&tcp->poll))  return fut_err;
            i64_t ret = send                             (
                tcp->tcp           ,
                par->buf + par->ret,
                par->len - par->ret,
                tcp->flag
            );

            if (ret == -1) return fut_err;
            par->ret += ret;

            if (par->ret >= par->len) return fut_ready;
            return fut_pend;
}

u64_t
    tcp_send_do_ret
        (io_res* par)                                                   {
            if (trait_of(par) != io_res_t) return 0; tcp* tcp = par->dev;
            if (trait_of(tcp) != tcp_t)    return 0;
            return par->ret;
}

fut_ops tcp_conn_do = make_fut_ops (tcp_conn_do_poll, tcp_conn_do_ret);
fut_ops tcp_send_do = make_fut_ops (tcp_send_do_poll, tcp_send_do_ret);
fut_ops tcp_recv_do = make_fut_ops (tcp_recv_do_poll, tcp_recv_do_ret);

bool_t 
	tcp_new
		(tcp* par_tcp, u32_t par_count, va_list par)		     	                    {
			io_sched  *sched = null_t; if (par_count > 0) sched = va_arg(par, io_sched*);
			obj_trait *af    = null_t; if (par_count > 1) af    = va_arg(par, void*)    ;
			if (trait_of(sched) != io_sched_t) return false_t;
			if (!af)                                         {
                par_tcp->sched = ref (sched);
                return true_t;
			}

            par_tcp->sched = ref (sched);
			if (!tcp_open(par_tcp, af)) {
			    del    (sched);
                return false_t;
            }

			return true_t;
}

bool_t 
	tcp_clone
		(tcp* par, tcp* par_clone) {
			return false_t;
}

void
	tcp_del
		(tcp* par)         {
            tcp_close(par) ;
			del(par->sched);
}

bool_t
    tcp_open
        (tcp* par, obj_trait* par_af)                              {
            if (trait_of(par) != tcp_t) return false_t; int af = -1;
            if (par_af == v6_t) af = AF_INET6;
            if (par_af == v4_t) af = AF_INET ;
            if (af == -1) return false_t;
            if (par->tcp) return true_t ;

            par->tcp = socket(af, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
            if (par->tcp <= 0)                                                goto open_err;
            if (!make_at(&par->poll, io_poll) from (2, par->sched, par->tcp)) goto open_err;
            return true_t;
    open_err:
            close(par->tcp);
            par->tcp   = 0;
            return false_t;

}

fut*
	tcp_conn
		(tcp* par, end* par_end)				              {
			if (trait_of(par_end) != end_t)      return null_t;
			if (trait_of(par)     != tcp_t)      return null_t;
			if (!tcp_open(par, end_af(par_end))) return null_t;

            io_res *res = make (io_res) from (3, par, null_t, 0)   ;
            fut    *ret = make (fut)    from (2, &tcp_conn_do, res);
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;

            connect (par->tcp, &par_end->all, par_end->len);
            if (errno != EINPROGRESS)                      {
                del     (res);
                del     (ret);
                return null_t;
            }

            io_poll_mask_out(&par->poll, true_t);
            del   (res);
            return ret ;
}

void 
	tcp_close
		(tcp* par)                              {
		    if   (trait_of(par) != tcp_t) return;
            close(par->tcp)  ;
            del  (&par->poll);
            par->tcp = 0;
}

fut*
	tcp_send
		(tcp* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != tcp_t) return null_t;
			if (!len)				     return null_t;
			if (!buf)				     return null_t;

			io_res *res = make (io_res) from (3, self, buf, len)   ;
			fut    *ret = make (fut)    from (2, &tcp_send_do, res);
			if (trait_of(res) != io_res_t) return null_t;
			if (trait_of(ret) != fut_t)    return null_t;
            del   (res);
            return ret ;
}

fut*
	tcp_recv
		(tcp* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != tcp_t) return null_t;
            if (!len)                    return null_t;
            if (!buf)                    return null_t;

            io_res *res = make (io_res) from (3, self, buf, len)   ;
            fut    *ret = make (fut)    from (2, &tcp_recv_do, res);
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            del   (res);
            return ret ;
}