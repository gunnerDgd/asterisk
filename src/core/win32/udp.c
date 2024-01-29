#include "udp.h"
#include "this.h"

obj_trait udp_trait     = {
    .on_new   = &udp_new  ,
    .on_clone = &udp_clone,
    .on_ref   =          0,
    .on_del   = &udp_del  ,
    .size     = sizeof(udp)
};

obj_trait *udp_t = &udp_trait;

bool_t 
    udp_new
        (udp* par_udp, u32_t par_count, va_list par)                      {
            io_run* run = 0; if (par_count > 0) run = va_arg(par, io_run*);
            if (!run)                      return false_t;
            if (trait_of(run) != io_run_t) return false_t;
            par_udp->udp = WSASocket (
                AF_INET            , 
                SOCK_DGRAM         , 
                IPPROTO_UDP        ,
                0                  , 
                0                  ,
                WSA_FLAG_OVERLAPPED
            );

            if (par_udp->udp == INVALID_SOCKET) return false_t;
            par_udp->udp_io = CreateIoCompletionPort (
                par_udp->udp,
                run->hnd    ,
                run         ,
                0
            );

            if (!par_udp->udp_io)        {
                closesocket(par_udp->udp);
                return false_t;
            }

            par_udp->run = ref(run);
            return true_t;
}

bool_t 
    udp_clone
        (udp* par, udp* par_clone) {
            return false_t;
}

void   
    udp_del
        (udp* par)             {
            del      (par->run);
            udp_close(par)     ;
            return;
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
        (udp* par, v4* par_v4)                                                         {
            if (!par)                                                    return false_t;
            if (!par_v4)                                                 return false_t;

            if (trait_of(par)    != udp_t)                               return false_t;
            if (trait_of(par_v4) != v4_t)                                return false_t;
            if (bind(par->udp, &par_v4->v4, sizeof(struct sockaddr_in))) return false_t;

            return true_t;
}

void   
    udp_close
        (udp* par)               {
            closesocket(par->udp);
            del        (par->run);

            par->udp = INVALID_SOCKET;
}

fut*
    udp_send
        (udp* par, u8_t* par_buf, u64_t par_len)   {
            if (!par_buf)                  return 0;
            if (!par_len)                  return 0;
            if (!par)                      return 0;
            if (trait_of(par) != udp_t)    return 0;

            io_res* ret = make (io_res_t) from (1, par->run);
            if (!ret)                      return 0;
            if (trait_of(ret) != io_res_t) return 0;

			WSABUF buf = { .buf = par_buf, .len = par_len };
			i32_t  res = WSASend (
                par->udp ,
                &buf     ,
                1        ,
                0        ,
                par->flag,
                &ret->res,
                0
            );

            fut* fut = io_res_fut(ret);
            del (ret);
			if (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                ret->stat = fut_err;
				return fut ;
			}
			return fut ;
}

fut*
    udp_send_to
        (udp* par, u8_t* par_buf, u64_t par_len, obj* par_addr)                                   {
            if (trait_of(par_addr) == v4_t) return udp_send_to_v4(par, par_buf, par_len, par_addr);
            return 0;
}

fut*
    udp_send_to_v4
        (udp* par, u8_t* par_buf, u64_t par_len, v4* par_v4) {
            if (!par_buf)                  return 0;
            if (!par_len)                  return 0;
            if (!par_v4)                   return 0;
            if (!par)                      return 0;

            if (trait_of(par_v4) != v4_t)  return 0;
            if (trait_of(par)    != udp_t) return 0;

			io_res *ret = make (io_res_t) from (1, par->run);
			if (!ret)                      return 0;
            if (trait_of(ret) != io_res_t) return 0;
			WSABUF buf = { .buf = par_buf, .len = par_len };
			i32_t  res = WSASendTo (
                par->udp          ,
                &buf              ,
                1                 ,
                0                 ,
                0                 ,
                &par_v4->v4       ,
                sizeof(par_v4->v4),
                &ret->res         ,
                0
            );

			fut* fut = io_res_fut(ret);
            del (ret);
			if (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                ret->stat = fut_err;
				return fut ;
			}
			return fut ;
}

fut*
    udp_recv
        (udp* par, u8_t* par_buf, u64_t par_len)   {
			if (!par_buf)                  return 0;
            if (!par_len)                  return 0;
            if (!par)                      return 0;
            if (trait_of(par) != udp_t)    return 0;

            io_res* ret = make (io_res_t) from (1, par->run);
            if (!ret)                      return 0;
            if (trait_of(ret) != io_res_t) return 0;

			WSABUF buf = { .buf = par_buf, .len = par_len };
			i32_t  res = WSARecv                           (
                par->udp  ,
                &buf      ,
                1         ,
                0         ,
                &par->flag,
                &ret->res ,
                0
            );

			fut* fut = io_res_fut(ret);
            del (ret);
			if (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                ret->stat = fut_err;
				return fut ;
			}
			return fut ;
}

fut*
    udp_recv_from
        (udp* par, u8_t* par_buf, u64_t par_len, obj* par_addr)                                   {
            if (trait_of(par_addr) == v4_t) return udp_send_to_v4(par, par_buf, par_len, par_addr);
            return 0;
}

fut*
    udp_recv_from_v4
        (udp* par, u8_t* par_buf, u64_t par_len, v4* par_v4) {
			if (!par_buf)                  return 0;
            if (!par_len)                  return 0;
            if (!par_v4)                   return 0;
            if (!par)                      return 0;

            if (trait_of(par_v4) != v4_t)  return 0;
            if (trait_of(par)    != udp_t) return 0;

			io_res *ret = make (io_res_t) from (1, par->run);
			if (!ret)                      return 0;
            if (trait_of(ret) != io_res_t) return 0;
			WSABUF buf = { .buf = par_buf, .len = par_len };
			i32_t  res = WSARecvFrom (
                par->udp          ,
                &buf              ,
                1                 ,
                0                 ,
                0                 ,
                &par_v4->v4       ,
                sizeof(par_v4->v4),
                &ret->res         ,
                0
            );

			fut* fut = io_res_fut(ret);
            del (ret);
			if (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                ret->stat = fut_err;
				return fut ;
			}
			return fut ;
}