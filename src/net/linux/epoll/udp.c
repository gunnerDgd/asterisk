#include "udp.h"
#include "v4.h"
#include "v6.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

obj_trait udp_trait     = {
    .on_new   = &udp_new  ,
    .on_clone = &udp_clone,
    .on_ref   =          0,
    .on_del   = &udp_del  ,
    .size     = sizeof(udp)
};

obj_trait *udp_t = &udp_trait;

i64_t
    udp_do_recv
        (udp* par, u8_t* par_buf, u64_t par_len, void* par_arg) {
            if (!par)                     return -1       ;
            if (trait_of(par) != udp_t)   return -1       ;
            if (io_poll_hang(&par->poll)) return -1       ;
            if (io_poll_err (&par->poll)) return -1       ;
            if (!io_poll_in(&par->poll))  goto   recv_pend;
            i64_t ret = recv                              (
                par->udp ,
                par_buf  ,
                par_len  ,
                par->flag
            );

            if (ret > 0) errno = 0;
            return ret;
    recv_pend:
            errno = EAGAIN;
            return 0;
}

i64_t
    udp_do_recv_from
        (udp* par, u8_t* par_buf, u64_t par_len, end* par_end) {
            if (!par)                       return -1;
            if (trait_of(par)     != udp_t) return -1;
            if (trait_of(par_end) != end_t) return -1;
            i64_t ret = recvfrom                     (
                par->udp     ,
                par_buf      ,
                par_len      ,
                par->flag    ,
                &par_end->end,
                &par_end->len
            );

            return ret;
}

i64_t
    udp_do_send
        (udp* par, u8_t* par_buf, u64_t par_len, void* par_arg) {
            if (!par)                     return -1;
            if (trait_of(par) != udp_t)   return -1;
            if (io_poll_hang(&par->poll)) return -1;
            if (io_poll_err (&par->poll)) return -1;
            i64_t ret = send                       (
                par->udp ,
                par_buf  ,
                par_len  ,
                par->flag
            );

            if (ret > 0) errno = 0;
            return ret;
}

i64_t
    udp_do_send_to
        (udp* par, u8_t* par_buf, u64_t par_len, end* par_end) {
            if (!par)                   return -1;
            if (trait_of(par) != udp_t) return -1;
            i64_t ret = sendto                   (
                par->udp     ,
                par_buf      ,
                par_len      ,
                par->flag    ,
                &par_end->end,
                par_end->len
            );

            return ret;
}

bool_t 
    udp_new
        (udp* par_udp, u32_t par_count, va_list par)                              {
            io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
            if (!sched)                        sched = this_io_sched();
            if (!sched)                        return false_t;
            if (trait_of(sched) != io_sched_t) return false_t;
            par_udp->udp = socket                            (
                AF_INET6                  ,
                SOCK_DGRAM | SOCK_NONBLOCK,
                IPPROTO_UDP
            );

            if (par_udp->udp < 0) return false_t;
            par_udp->sched = ref(sched);
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
    udp_conn
        (udp* par, end* par_end)                          {
            if (!par_end)                   return false_t;
            if (!par)                       return false_t;
            if (trait_of(par)     != udp_t) return false_t;
            if (trait_of(par_end) != end_t) return false_t;

            int    udp = socket(par_end->end.type, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
            close (par->udp)  ;
            del   (&par->poll);

            if (udp <= 0)                                                  return false_t;
            if (bind(udp, &par_end->end, par_end->len))                    return false_t;
            if (!make_at(&par->poll, io_poll_t) from (2, par->sched, udp)) return false_t;

            par->udp = udp ;
            return true_t;
}

void   
    udp_close
        (udp* par)                            {
            if (!par)                   return;
            if (trait_of(par) != udp_t) return;
            close(par->udp)  ;
            del  (&par->poll);
            par->udp = 0;
}

fut*
    udp_send
        (udp* par, u8_t* par_buf, u64_t par_len)           {
            if (!par_len)				           return 0;
            if (!par_buf)				           return 0;
            if (!par)					           return 0;
            if (trait_of(par)        != udp_t)	   return 0;
            if (trait_of(&par->poll) != io_poll_t) return 0;
            io_res *ret = make (io_res_t) from             (
                4          ,
                par        ,
                udp_do_send,
                par_buf    ,
                par_len
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret); del (ret);
            if (trait_of(fut) != fut_t)    return 0;
            return fut;
}

fut*
    udp_send_to
        (udp* par, u8_t* par_buf, u64_t par_len, end* par_end) {
            if (!par_len)				return 0;
            if (!par_buf)				return 0;
            if (!par)					return 0;
            if (trait_of(par) != udp_t)	return 0;
            io_res *ret = make (io_res_t) from  (
                5             ,
                par           ,
                udp_do_send_to,
                par_buf       ,
                par_len       ,
                par_end
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret); del (ret);
            if (trait_of(fut) != fut_t)    return 0;
            return fut;
}

fut*
    udp_recv
        (udp* par, u8_t* par_buf, u64_t par_len)           {
            if (!par_len)				           return 0;
            if (!par_buf)				           return 0;
            if (!par)					           return 0;
            if (trait_of(par)        != udp_t)	   return 0;
            if (trait_of(&par->poll) != io_poll_t) return 0;
            io_res *ret = make (io_res_t) from             (
                4          ,
                par        ,
                udp_do_recv,
                par_buf    ,
                par_len
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret); del (ret);
            if (trait_of(fut) != fut_t)    return 0;
            return fut;
}

fut*
    udp_recv_from
        (udp* par, u8_t* par_buf, u64_t par_len, end* par_end) {
            if (!par_len)				return 0;
            if (!par_buf)				return 0;
            if (!par)					return 0;
            if (trait_of(par) != udp_t)	return 0;
            io_res *ret = make (io_res_t) from  (
                5               ,
                par             ,
                udp_do_recv_from,
                par_buf         ,
                par_len         ,
                par_end
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret); del (ret);
            if (trait_of(fut) != fut_t)    return 0;
            return fut;
}