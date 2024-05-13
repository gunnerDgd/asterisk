#include "udp.h"

#include "v4.h"
#include "v6.h"

obj_trait udp_trait = make_trait (
    udp_new    ,
    udp_clone  ,
    null_t     ,
    udp_del    ,
    sizeof(udp),
    null_t
);

obj_trait *udp_t = &udp_trait;

bool_t 
    udp_new
        (udp* par_udp, u32_t par_count, va_list par)                                   {
            io_sched* sched = null_t; if (par_count > 0) sched = va_arg(par, io_sched*);
            if (trait_of(sched) != io_sched_t) sched = this_io_sched();
            if (trait_of(sched) != io_sched_t) return false_t;
            par_udp->sched  = ref(sched)    ;
            par_udp->udp_io = 0             ;
            par_udp->udp    = INVALID_SOCKET;
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
            return;
}

bool_t 
    udp_open
        (udp* par, obj_trait* par_af)                                  {
            if (trait_of(par) != udp_t) return false_t; int af = -1;
            if (par_af == v4_t) af = AF_INET  ;
            if (par_af == v6_t) af = AF_INET6 ;
            if (af == -1)       return false_t;

            if (par->udp != INVALID_SOCKET) return true_t;
            par->udp = WSASocket                         (
                af                ,
                SOCK_DGRAM        ,
                IPPROTO_UDP       ,
                0                 ,
                0                 ,
                WSA_FLAG_OVERLAPPED
            );

            if (par->udp == INVALID_SOCKET) return false_t;
            par->udp_io = CreateIoCompletionPort          (
                par->udp       ,
                par->sched->hnd,
                par->sched     ,
                0
            );

            if (!par->udp_io)            {
                closesocket(par->udp)    ;
                par->udp = INVALID_SOCKET;
                return false_t;
            }
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
            closesocket(par->udp)    ;
            par->udp = INVALID_SOCKET;
}

fut*
    udp_send
        (udp* par, u8_t* par_buf, u64_t par_len)     {
            if (trait_of(par) != udp_t) return null_t;
            if (!par_buf)               return null_t;
            if (!par_len)               return null_t;
            

            io_res *ret = make (io_res) from (1, par->sched); if (trait_of(ret) != io_res_t) return 0;
			WSABUF  buf = { .buf = par_buf, .len = par_len };
			i32_t   res = WSASend                           (
                par->udp ,
                &buf     ,
                1        ,
                0        ,
                par->flag,
                &ret->res,
                0
            );

            fut* fut = io_res_fut(ret);
			if  (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                 ret->stat = fut_err          ;
                 ret->ret  = WSAGetLastError();
                 del   (ret);
				 return fut ;
			}

            del   (ret);
			return fut ;
}

fut*
    udp_send_to
        (udp* par, u8_t* par_buf, u64_t par_len, end* par_end) {
            if (trait_of(par_end) != end_t) return 0;
            if (trait_of(par)     != udp_t) return 0;
            if (!par_buf)                   return 0;
            if (!par_len)                   return 0;

            io_res *ret = make (io_res) from (1, par->sched); if (trait_of(ret) != io_res_t) return 0; 
			WSABUF  buf = { .buf = par_buf, .len = par_len };
			i32_t   res = WSASendTo                         (
                par->udp     ,
                &buf         ,
                1            ,
                0            ,
                0            ,
                &par_end->all,
                par_end->len ,
                &ret->res    ,
                0
            );

			fut* fut = io_res_fut(ret);
			if  (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                 ret->stat = fut_err          ;
                 ret->ret  = WSAGetLastError();
                 del   (ret);
				 return fut;
			}

            del   (ret);
			return fut ;
}

fut*
    udp_recv
        (udp* par, u8_t* par_buf, u64_t par_len) {
            if (trait_of(par) != udp_t) return 0;
			if (!par_buf)               return 0;
            if (!par_len)               return 0;

            io_res *ret = make (io_res) from (1, par->sched); if (trait_of(ret) != io_res_t) return 0;
			WSABUF  buf = { .buf = par_buf, .len = par_len };
			i32_t   res = WSARecv                           (
                par->udp  ,
                &buf      ,
                1         ,
                0         ,
                &par->flag,
                &ret->res ,
                0
            );

			fut* fut = io_res_fut(ret);
			if  (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                 ret->stat = fut_err          ;
                 ret->ret  = WSAGetLastError();
                 del   (ret);
				 return fut ;
			}

            del   (ret);
			return fut ;
}

fut*
    udp_recv_from
        (udp* par, u8_t* par_buf, u64_t par_len, end* par_end) {
            if (trait_of(par_end) != end_t) return 0;
            if (trait_of(par)     != udp_t) return 0;
            if (!par_buf)                   return 0;
            if (!par_len)                   return 0;

			io_res *ret = make (io_res) from (1, par->sched); if (trait_of(ret) != io_res_t) return 0;
			WSABUF  buf = { .buf = par_buf, .len = par_len };
			i32_t   res = WSARecvFrom                       (
                par->udp     ,
                &buf         ,
                1            ,
                0            ,
                0            ,
                &par_end->all,
                par_end->len ,
                &ret->res    ,
                0
            );

			fut* fut = io_res_fut(ret);
			if  (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                 ret->stat = fut_err          ;
                 ret->ret  = WSAGetLastError();
                 del   (ret);
				 return fut ;
			}

            del   (ret);
			return fut ;
}