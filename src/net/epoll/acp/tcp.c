#include "tcp.h"

#include "../acp.h"
#include "../v4.h"
#include "../v6.h"

u64_t
    do_tcp_poll
        (io_res* self)                                                           {
            if (trait_of(self) != io_res_t) return fut_err; tcp *dev = self->dev;
            if (trait_of(dev)  != tcp_t)    return fut_err; tcp *tcp = self->arg;
            if (trait_of(tcp)  != tcp_t)    return fut_err;
            if (io_poll_hang(&dev->poll))   goto err;
            if (io_poll_err (&dev->poll))   goto err;

            if (!io_poll_in(&dev->poll)) io_run_flush(dev->run);
            if (!io_poll_in(&dev->poll)) return fut_pend;
            tcp->tcp = accept(dev->tcp, 0, 0);

            if (!make_at(&tcp->poll, io_poll) from (2, tcp->run, tcp->tcp)) goto err;
            self->ret = (u64_t) tcp;
            return fut_ready;
    err:    self->arg = null_t;
            self->ret = 0ull;
            del (tcp);
            return fut_err;
}

any_t
    do_tcp_ret
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return null_t; tcp *dev = self->dev;
            if (trait_of(dev)  != tcp_t)    return null_t;
            tcp* ret = (tcp*) self->ret;

            if (trait_of(ret) != tcp_t) return null_t;
            return (any_t) self->ret;
}

fut_ops
    do_tcp = make_fut_ops (
        do_tcp_poll,
        do_tcp_ret
);

bool_t
    do_tcp_open
        (struct acp* self)                                                    {
            if (trait_of (self) != acp_t)                       return false_t;
            if (!make_at (&self->tcp, tcp) from (1, self->run)) return false_t;
            if (!tcp_open(&self->tcp, end_af(self->end)))       return false_t;
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
        (struct acp* self)                     {
            if (trait_of(self) != acp_t) return;
            tcp_close(&self->tcp);
}

fut*
    do_tcp_fut
		(struct acp* self)                            {
			if (trait_of(self) != acp_t) return null_t;
            tcp    *arg = make (tcp) from (1, self->run);
			tcp    *dev = &self->tcp;
			io_res *res = null_t;
		    fut    *ret = null_t;

		    if (trait_of(arg) != tcp_t) goto err;
		    if (trait_of(dev) != tcp_t) goto err;
            res = make (io_res) from (4, dev, null_t, 0, arg);
            ret = make (fut)    from (2, &do_tcp, res);

            if (trait_of(res) != io_res_t) goto err;
            if (trait_of(ret) != fut_t)    goto err;
            del   (res);
            return ret ;
    err:    del     (dev);
            del     (res);
            del     (ret);
            return null_t;
}