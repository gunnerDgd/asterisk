 #include "udp.h"
#include "v4.h"
#include "v6.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static u64_t
    do_recv_poll
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; udp* udp = self->dev;
            if (trait_of(udp)  != udp_t)    return fut_err;
            if (io_poll_hang(&udp->poll))   return fut_err;
            if (io_poll_err (&udp->poll))   return fut_err;

            if (!io_poll_in (&udp->poll)) io_run_flush(udp->run);
            if (!io_poll_in (&udp->poll)) return fut_pend;
            i64_t ret = recv                             (
                udp ->udp,
                self->buf,
                self->len,
                udp ->flag
            );

            if (ret == -1) return fut_err ; self->ret += ret;
            if (ret ==  0) return fut_pend;
            return fut_ready;
}

static u64_t
    do_recv_ret
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; udp* udp = self->dev;
            if (trait_of(udp)  != udp_t)    return fut_err;
            return self->ret;
}

static fut_ops
    do_recv = make_fut_ops (
        do_recv_poll,
        do_recv_ret
);



static i64_t
    do_recv_from_poll
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; udp *udp = self->dev;
            if (trait_of(udp)  != udp_t)    return fut_err; end *end = self->arg;
            if (trait_of(end)  != end_t)    return fut_err;
            if (io_poll_hang(&udp->poll))  return fut_err;
            if (io_poll_err (&udp->poll))  return fut_err;

            if (!io_poll_in (&udp->poll)) io_run_flush(udp->run);
            if (!io_poll_in (&udp->poll)) return fut_pend;
            i64_t ret = recvfrom                         (
                udp->udp ,
                self->buf,
                self->len,
                udp->flag,
                &end->all,
                &end->len
            );

            if (ret == -1) return fut_err ; self->ret += ret;
            if (ret ==  0) return fut_pend;
            return fut_ready;
}

static u64_t
    do_recv_from_ret
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; udp* udp = self->dev;
            if (trait_of(udp)  != udp_t)    return fut_err;
            return self->ret;
}

static fut_ops
    do_recv_from = make_fut_ops (
        do_recv_from_poll,
        do_recv_from_ret
);


static u64_t
    do_send_poll
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; udp *udp = self->dev;
            if (trait_of(udp)  != udp_t)    return fut_err;
            if (io_poll_hang(&udp->poll))  return fut_err;
            if (io_poll_err (&udp->poll))  return fut_err;
            self->ret = send                              (
                udp ->udp,
                self->buf,
                self->len,
                udp ->flag
            );

            if (self->ret == -1) return fut_err  ;
            if (self->ret)       return fut_ready;
            return fut_pend;
}

static u64_t
    do_send_ret
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; udp* udp = self->dev;
            if (trait_of(udp)  != udp_t)    return fut_err;
            return self->ret;
}

 static fut_ops
    do_send = make_fut_ops (
        do_send_poll,
        do_send_ret
);


static i64_t
    do_send_to_poll
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; udp *udp = self->dev;
            if (trait_of(udp)  != udp_t)    return fut_err; end *end = self->arg;
            if (trait_of(end)  != end_t)    return fut_err;
            self->ret = sendto                            (
                udp ->udp ,
                self->buf ,
                self->len ,
                udp ->flag,
                &end->all ,
                end ->len
            );

            if (self->ret == -1) return fut_err  ;
            if (self->ret)       return fut_ready;
            return fut_pend;
}

static u64_t
    do_send_to_ret
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; udp* udp = self->dev;
            if (trait_of(udp)  != udp_t)    return fut_err;
            return self->ret;
}



static fut_ops
    do_send_to = make_fut_ops (
        do_send_to_poll,
        do_send_to_ret
);


static bool_t
    do_new
        (udp* self, u32_t count, va_list arg)                               {
            io_run    *run = null_t; if (count > 0) run = va_arg(arg, any_t);
            obj_trait *af  = null_t; if (count > 1) af  = va_arg(arg, any_t);
            if (trait_of(run) != io_run_t) run = this_io_run();
            if (trait_of(run) != io_run_t) return false_t;
            self->run = ref (run);

            if (!af) return true_t;
            if (!udp_open(self, af)) {
                del      (run);
                return false_t;
            }

            return true_t;
}

static bool_t
    do_clone
        (udp* self, udp* clone) {
            return false_t;
}

static void
    do_del
        (udp* self)             {
            udp_close(self)     ;
            del      (self->run);
}

 static obj_trait
    do_obj = make_trait (
        do_new     ,
        do_clone   ,
        null_t     ,
        do_del     ,
        sizeof(udp),
        null_t
 );

 obj_trait *udp_t = &do_obj;

bool_t
    udp_open
        (udp* self, obj_trait* af)                     {
            if (trait_of(self) != udp_t) return false_t;
            int soc = SOCK_DGRAM | SOCK_NONBLOCK;
            int pro = IPPROTO_UDP;
            int dom = -1;

            if (af == v6_t) dom = AF_INET6;
            if (af == v4_t) dom = AF_INET ;
            if (dom == -1)  return false_t;
            if (self->udp)  return true_t ;

            self->udp = socket(dom, soc, pro);
            if (!make_at(&self->poll, io_poll) from (2, self->run, self->udp)) goto err;
            return true_t;
    err:    close(self->udp);
            self->udp    = 0;
            return false_t;
}

bool_t 
    udp_conn
        (udp* self, end* end)                          {
            if (trait_of(self) != udp_t) return false_t;
            if (trait_of(end)  != end_t) return false_t;

            if (!udp_open(self, end_af(end)))         return false_t;
            if (bind(self->udp, &end->all, end->len)) return false_t;
            return true_t;
}

void   
    udp_close
        (udp* self)                            {
            if (trait_of(self) != udp_t) return;
            close(self->udp)  ;
            del  (&self->poll);
            self->udp = 0;
}

fut*
    udp_send
        (udp* self, u8_t* buf, u64_t len)                        {
            if (trait_of(&self->poll) != io_poll_t) return null_t;
            if (trait_of(self)        != udp_t)     return null_t;
            if (!len)                               return null_t;
            if (!buf)                               return null_t;

            io_res *res = make (io_res) from (3, self, buf, len);
            fut    *ret = make (fut)    from (2, &do_send, res);
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            fut_poll(ret);
            del     (res);
            return   ret ;
}

fut*
    udp_send_to
        (udp* self, u8_t* buf, u64_t len, end* end)              {
            if (trait_of(&self->poll) != io_poll_t) return null_t;
            if (trait_of(self)        != udp_t)	    return null_t;
            if (!len)                               return null_t;
            if (!buf)				                return null_t;

            io_res *res = make (io_res) from (5, self, buf, len, end);
            fut    *ret = make (fut)    from (2, &do_send_to, res);
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            fut_poll(ret);
            del     (res);
            return   ret ;
}

fut*
    udp_recv
        (udp* self, u8_t* buf, u64_t len)                        {
            if (trait_of(&self->poll) != io_poll_t) return null_t;
            if (trait_of(self)        != udp_t)     return null_t;
            if (!len)                               return null_t;
            if (!buf)                               return null_t;

            io_res *res = make (io_res) from (3, self, buf, len);
            fut    *ret = make (fut)    from (2, &do_recv, res) ;
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            del     (res);
            return   ret ;
}

fut*
    udp_recv_from
        (udp* self, u8_t* buf, u64_t len, end* end)              {
            if (trait_of(&self->poll) != io_poll_t) return null_t;
            if (trait_of(self)        != udp_t)	    return null_t;
            if (!len)                               return null_t;
            if (!buf)                               return null_t;

            io_res *res = make (io_res) from (4, self, buf, len, end);
            fut    *ret = make (fut)    from (2, &do_recv_from, res);
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            del     (res);
            return   ret ;
}