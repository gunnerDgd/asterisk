#include "tcp.h"
#include "v4.h"
#include "v6.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
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
        (io_res* self)                                                    {
            if (trait_of(self) != io_res_t) goto err; tcp* tcp = self->dev;
            if (trait_of(tcp)  != tcp_t)    goto err;

            if (!io_poll_out(&tcp->poll)) io_sched_run (tcp->sched);
            if (!io_poll_out(&tcp->poll)) goto pend;
            if (io_poll_hang(&tcp->poll)) goto err;
            if (io_poll_err (&tcp->poll)) goto err;
            io_poll_mask_out(&tcp->poll, false_t);
            self->stat = fut_ready;
            return fut_ready;
    pend:   self->stat = fut_pend;
            return fut_pend;
    err:    self->stat = fut_err;
            return fut_err;
}

void*
    tcp_conn_do_ret
        (io_res* par)                                                        {
            if (trait_of(par) != io_res_t) return null_t; tcp* tcp = par->dev;
            if (trait_of(tcp) != tcp_t)    return null_t;
            if (par->stat != fut_ready)    return null_t;
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
		(tcp* self, u32_t count, va_list arg)		     	                        {
			io_sched  *sched = null_t; if (count > 0) sched = va_arg(arg, io_sched*);
			obj_trait *af    = null_t; if (count > 1) af    = va_arg(arg, void*)    ;
			if (trait_of(sched) != io_sched_t) sched = this_io_sched();
			if (trait_of(sched) != io_sched_t) return false_t;
			if (!af)                                         {
                self->sched = ref (sched);
                return true_t;
			}

            self->sched = ref (sched);
			if (!tcp_open(self, af)) {
			    del    (sched);
                return false_t;
            }

			return true_t;
}

bool_t 
	tcp_clone
		(tcp* self, tcp* clone) {
			return false_t;
}

void
	tcp_del
		(tcp* self)         {
            tcp_close(self) ;
			del(self->sched);
}

bool_t
    tcp_open
        (tcp* self, obj_trait* af)                                   {
            if (trait_of(self) != tcp_t) return false_t; int res = -1;
            if (af == v6_t) res = AF_INET6;
            if (af == v4_t) res = AF_INET ;
            if (res == -1)  return false_t;
            if (self->tcp)  return true_t ;

            self->tcp = socket(res, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
            if (self->tcp <= 0)                                                  goto err;
            if (!make_at(&self->poll, io_poll) from (2, self->sched, self->tcp)) goto err;
            return true_t;
    err:    close(self->tcp);
            self->tcp    = 0;
            return false_t;

}

fut*
	tcp_conn
		(tcp* self, end* end)				               {
            if (trait_of(self) != tcp_t)      return null_t;
			if (trait_of(end)  != end_t)      return null_t;
			if (!tcp_open(self, end_af(end))) return null_t;
            io_poll_mask_out(&self->poll, true_t);
			io_res *res = null_t;
			fut    *ret = null_t;

            res = make (io_res) from (3, self, null_t, 0)  ;
            ret = make (fut)    from (2, &tcp_conn_do, res);
            if (trait_of(res) != io_res_t) goto err;
            if (trait_of(ret) != fut_t)    goto err;

            connect (self->tcp, &end->all, end->len);
            if (errno != EINPROGRESS) goto err;
            del   (res);
            return ret ;
    err:    del      (res);
            del      (ret);
            return false_t;
}

void 
	tcp_close
		(tcp* self)                              {
		    if   (trait_of(self) != tcp_t) return;
            close(self->tcp)  ;
            del  (&self->poll);
            self->tcp = 0;
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