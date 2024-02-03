#include "udp.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <stdio.h>

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
            if (!par)                   return -1;
            if (trait_of(par) != udp_t) return -1;
            i64_t ret = recv                     (
                par->udp ,
                par_buf  ,
                par_len  ,
                par->flag
            );

            if (ret > 0) errno = 0;
            return ret;
}

i64_t
    udp_do_recv_from_v4
        (udp* par, u8_t* par_buf, u64_t par_len, v4* par_v4) {
            if (!par)                      return -1;
            if (trait_of(par)    != udp_t) return -1;
            if (trait_of(par_v4) != v4_t)  return -1;
            i64_t ret = recvfrom                    (
                par->udp     ,
                par_buf      ,
                par_len      ,
                par->flag    ,
                &par_v4->v4  ,
                &par_v4->size
            );

            return ret;
}

i64_t
    udp_do_send
        (udp* par, u8_t* par_buf, u64_t par_len, void* par_arg) {
            if (!par)                   return -1;
            if (trait_of(par) != udp_t) return -1;
            i64_t ret = send                     (
                par->udp ,
                par_buf  ,
                par_len  ,
                par->flag
            );

            return ret;
}

i64_t
    udp_do_send_to_v4
        (udp* par, u8_t* par_buf, u64_t par_len, v4* par_v4) {
            if (!par)                   return -1;
            if (trait_of(par) != udp_t) return -1;
            i64_t ret = sendto                   (
                par->udp    ,
                par_buf     ,
                par_len     ,
                par->flag   ,
                &par_v4->v4 ,
                sizeof(par_v4->v4)
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
                AF_INET                   ,
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
            del      (par->sched);
            udp_close(par)       ;
}

bool_t 
    udp_conn
        (udp* par, obj* par_conn)                          {
            if (!par_conn)                   return false_t;
            if (!par)                        return false_t;

            if (trait_of(par)      != udp_t) return false_t;
            if (trait_of(par_conn) == v4_t)  return udp_conn_v4(par, par_conn);
            return false_t;
}

bool_t 
    udp_conn_v4
        (udp* par, v4* par_v4)                                                 {
            if (!par_v4)                                         return false_t;
            if (!par)                                            return false_t;
            if (trait_of(par)    != udp_t)                       return false_t;
            if (trait_of(par_v4) != v4_t)                        return false_t;
            if (bind(par->udp, &par_v4->v4, sizeof(par_v4->v4))) return false_t;
            return true_t;
}

void   
    udp_close
        (udp* par)         {
            close(par->udp);
}

fut*
    udp_send
        (udp* par, u8_t* par_buf, u64_t par_len) {
            if (!par_len)				return 0;
            if (!par_buf)				return 0;
            if (!par)					return 0;
            if (trait_of(par) != udp_t)	return 0;
            io_res *ret = make (io_res_t) from (
                4          ,
                par        ,
                udp_do_send,
                par_buf    ,
                par_len
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
            if (trait_of(fut) != fut_t)    return 0; del (ret);
            return fut;
}

fut*
    udp_send_to
        (udp* par, u8_t* par_buf, u64_t par_len, obj* par_addr) {
            if (!par_len)				     return 0;
            if (!par_buf)				     return 0;
            if (!par)					     return 0;
            if (trait_of(par)      != udp_t) return 0;
            if (trait_of(par_addr) == v4_t)  return udp_send_to_v4(par, par_buf, par_len, par_addr);
            return 0;
}

fut*
    udp_send_to_v4
        (udp* par, u8_t* par_buf, u64_t par_len, v4* par_v4) {
            if (!par_len)				return 0;
            if (!par_buf)				return 0;
            if (!par)					return 0;
            if (trait_of(par) != udp_t)	return 0;
            io_res *ret = make (io_res_t) from (
                5                ,
                par              ,
                udp_do_send_to_v4,
                par_buf          ,
                par_len          ,
                par_v4
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
            if (trait_of(fut) != fut_t)    return 0; del (ret);
            return fut;
}

fut*
    udp_recv
        (udp* par, u8_t* par_buf, u64_t par_len) {
            if (!par_len)				return 0;
            if (!par_buf)				return 0;
            if (!par)					return 0;
            if (trait_of(par) != udp_t)	return 0;
            io_res *ret = make (io_res_t) from (
                4          ,
                par        ,
                udp_do_recv,
                par_buf    ,
                par_len
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
            if (trait_of(fut) != fut_t)    return 0; del (ret);
            return fut;
}

fut*
    udp_recv_from
        (udp* par, u8_t* par_buf, u64_t par_len, obj* par_addr) {
            if (!par_len)				     return 0;
            if (!par_buf)				     return 0;
            if (!par)					     return 0;
            if (trait_of(par)      != udp_t) return 0;
            if (trait_of(par_addr) == v4_t)  return udp_recv_from_v4(par, par_buf, par_len, par_addr);
            return 0;
}

fut*
    udp_recv_from_v4
        (udp* par, u8_t* par_buf, u64_t par_len, v4* par_v4) {
            if (!par_len)				return 0;
            if (!par_buf)				return 0;
            if (!par)					return 0;
            if (trait_of(par) != udp_t)	return 0;
            io_res *ret = make (io_res_t) from (
                5                  ,
                par                ,
                udp_do_recv_from_v4,
                par_buf            ,
                par_len            ,
                par_v4
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
            if (trait_of(fut) != fut_t)    return 0; del (ret);
            return fut;
}