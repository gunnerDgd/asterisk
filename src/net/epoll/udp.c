 #include "udp.h"
#include "v4.h"
#include "v6.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

obj_trait udp_trait = make_trait (
    udp_new    ,
    udp_clone  ,
    null_t     ,
    udp_del    ,
    sizeof(udp),
    null_t
);

obj_trait *udp_t = &udp_trait;

u64_t
    udp_recv_do_poll
        (io_res* self)                                                                  {
            if (trait_of(self) != io_res_t) return fut_err; udp* poll = (udp*) self->dev;
            if (trait_of(poll) != udp_t)    return fut_err;
            if (io_poll_hang(&poll->poll))  return fut_err;
            if (io_poll_err (&poll->poll))  return fut_err;

            if (!io_poll_in (&poll->poll)) io_sched_run(poll->sched);
            if (!io_poll_in (&poll->poll)) return fut_pend;
            i64_t ret = recv                              (
                poll->udp,
                self->buf,
                self->len,
                poll->flag
            );

            if (ret == -1) return fut_err ; self->ret += ret;
            if (ret ==  0) return fut_pend;
            return fut_ready;
}

u64_t
    udp_recv_do_ret
        (io_res* par)                                                         {
            if (trait_of(par) != io_res_t) return fut_err; udp* udp = par->dev;
            if (trait_of(udp) != udp_t)    return fut_err;
            return par->ret;
}

i64_t
    udp_recv_from_do_poll
        (io_res* par)                                                         {
            if (trait_of(par) != io_res_t) return fut_err; udp *udp = par->dev;
            if (trait_of(udp) != udp_t)    return fut_err; end *end = par->arg;
            if (trait_of(end) != end_t)    return fut_err;
            if (io_poll_hang(&udp->poll))  return fut_err;
            if (io_poll_err (&udp->poll))  return fut_err;

            if (!io_poll_in (&udp->poll)) io_sched_run(udp->sched);
            if (!io_poll_in (&udp->poll)) return fut_pend;
            i64_t ret = recvfrom                         (
                udp->udp ,
                par->buf ,
                par->len ,
                udp->flag,
                &end->all,
                &end->len
            );

            if (ret == -1) return fut_err ; par->ret += ret;
            if (ret ==  0) return fut_pend;
            return fut_ready;
}

u64_t
    udp_recv_from_do_ret
        (io_res* self)                                                                 {
            if (trait_of(self) != io_res_t) return fut_err; udp* ret = (udp)* self->dev;
            if (trait_of(ret)  != udp_t)    return fut_err;
            return self->ret;
}

u64_t
    udp_send_do_poll
        (io_res* self)                                                                  {
            if (trait_of(self) != io_res_t) return fut_err; udp *poll = (udp*) self->dev;
            if (trait_of(poll) != udp_t)    return fut_err;
            if (io_poll_hang(&poll->poll))  return fut_err;
            if (io_poll_err (&poll->poll))  return fut_err;
            self->ret = send                              (
                poll->udp,
                self->buf,
                self->len,
                poll->flag
            );

            if (self->ret == -1) return fut_err  ;
            if (self->ret)       return fut_ready;
            return fut_pend;
}

u64_t
    udp_send_do_ret
        (io_res* par)                                                         {
            if (trait_of(par) != io_res_t) return fut_err; udp* udp = par->dev;
            if (trait_of(udp) != udp_t)    return fut_err;
            return par->ret;
}

i64_t
    udp_send_to_do_poll
        (io_res* self)                                                                  {
            if (trait_of(self) != io_res_t) return fut_err; udp *poll = (udp*) self->dev;
            if (trait_of(poll) != udp_t)    return fut_err; end *end  =        self->arg;
            if (trait_of(end)  != end_t)    return fut_err;
            self->ret = sendto                            (
                poll->udp ,
                self->buf ,
                self->len ,
                poll->flag,
                &end->all ,
                end->len
            );

            if (self->ret == -1) return fut_err  ;
            if (self->ret)       return fut_ready;
            return fut_pend;
}

u64_t
    udp_send_to_do_ret
        (io_res* self)                                                                 {
            if (trait_of(self) != io_res_t) return fut_err; udp* ret = (udp*) self->dev;
            if (trait_of(ret)  != udp_t)    return fut_err;
            return self->ret;
}

fut_ops udp_recv_from_do = make_fut_ops (udp_recv_from_do_poll, udp_recv_from_do_ret);
fut_ops udp_recv_do      = make_fut_ops (udp_recv_do_poll     , udp_recv_do_ret)     ;
fut_ops udp_send_to_do   = make_fut_ops (udp_send_to_do_poll  , udp_send_to_do_ret)  ;
fut_ops udp_send_do      = make_fut_ops (udp_send_do_poll     , udp_send_do_ret)     ;

bool_t 
    udp_new
        (udp* par_udp, u32_t par_count, va_list par)                                    {
            io_sched  *sched = null_t; if (par_count > 0) sched = va_arg(par, io_sched*);
            obj_trait *af    = null_t; if (par_count > 1) af    = va_arg(par, void*)    ;
            if (trait_of(sched) != io_sched_t) return false_t;
            if (!af)                                         {
                par_udp->sched = ref (sched);
                return true_t;
            }

            par_udp->sched = ref (sched);
            if (!udp_open(par_udp, af)) {
                del    (sched);
                return false_t;
            }

            return true_t;
}

bool_t 
    udp_clone
        (udp* par, udp* par_clone) {
            return false_t;
}

void   
    udp_del
        (udp* par)               {
            udp_close(par)       ;
            del      (par->sched);
}

bool_t
    udp_open
        (udp* par, obj_trait* par_af)                              {
            if (trait_of(par) != udp_t) return false_t; int af = -1;
            if (par_af == v4_t) af = AF_INET ;
            if (par_af == v6_t) af = AF_INET6;
            if (af == -1) return false_t;
            if (par->udp) return true_t ;

            par->udp = socket(af, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
            if (par->udp <= 0)                                                goto open_err;
            if (!make_at(&par->poll, io_poll) from (2, par->sched, par->udp)) goto open_err;
            return true_t;
    open_err:
            close(par->udp);
            par->udp   = 0;
            return false_t;
}

bool_t 
    udp_conn
        (udp* par, end* par_end)                          {
            if (trait_of(par_end) != end_t) return false_t;
            if (trait_of(par)     != udp_t) return false_t;

            if (!udp_open(par, end_af(par_end)))             return false_t;
            if (bind(par->udp, &par_end->all, par_end->len)) return false_t;
            return true_t;
}

void   
    udp_close
        (udp* par)                            {
            if (trait_of(par) != udp_t) return;
            close(par->udp)  ;
            del  (&par->poll);
            par->udp = 0;
}

fut*
    udp_send
        (udp* par, u8_t* par_buf, u64_t par_len)                {
            if (trait_of(&par->poll) != io_poll_t) return null_t;
            if (trait_of(par)        != udp_t)	   return null_t;
            if (!par_len)				           return null_t;
            if (!par_buf)				           return null_t;

            io_res *res = make (io_res) from (3, par, par_buf, par_len);
            fut    *ret = make (fut)    from (2, &udp_send_do, res)    ;
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            fut_poll(ret);
            del     (res);
            return   ret ;
}

fut*
    udp_send_to
        (udp* par, u8_t* par_buf, u64_t par_len, end* par_end)  {
            if (trait_of(&par->poll) != io_poll_t) return null_t;
            if (trait_of(par)        != udp_t)	   return null_t;
            if (!par_len)				           return null_t;
            if (!par_buf)				           return null_t;

            io_res *res = make (io_res) from (5, par, par_buf, par_len, par_end);
            fut    *ret = make (fut)    from (2, &udp_send_to_do, res)          ;
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            fut_poll(ret);
            del     (res);
            return   ret ;
}

fut*
    udp_recv
        (udp* par, u8_t* par_buf, u64_t par_len)                {
            if (trait_of(&par->poll) != io_poll_t) return null_t;
            if (trait_of(par)        != udp_t)	   return null_t;
            if (!par_len)				           return null_t;
            if (!par_buf)				           return null_t;

            io_res *res = make (io_res) from (3, par, par_buf, par_len);
            fut    *ret = make (fut)    from (2, &udp_recv_do, res)    ;
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            del     (res);
            return   ret ;
}

fut*
    udp_recv_from
        (udp* par, u8_t* par_buf, u64_t par_len, end* par_end)  {
            if (trait_of(&par->poll) != io_poll_t) return null_t;
            if (trait_of(par)        != udp_t)	   return null_t;
            if (!par_len)				           return null_t;
            if (!par_buf)				           return null_t;

            io_res *res = make (io_res) from (4, par, par_buf, par_len, par_end);
            fut    *ret = make (fut)    from (2, &udp_recv_from_do, res)        ;
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;
            del     (res);
            return   ret ;
}