#include "udp.h"

#include "v4.h"
#include "v6.h"


static bool_t 
    do_new
        (udp* self, u32_t count, va_list arg)                            {
            io_run* run = null_t; if (count > 0) run = va_arg(arg, any_t);
            if (trait_of(run) != io_run_t) run = this_io_run();
            if (trait_of(run) != io_run_t) return false_t;

            self->udp = INVALID_SOCKET;
            self->run = ref(run)      ;
            self->ioc = null_t        ;
            return true_t;
}

static bool_t 
    do_clone
        (udp* self, udp* clone) {
            return false_t;
}

static void   
    do_del
        (udp* self)        {
            del (self->run);
            udp_close(self);
            return;
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

obj_trait* udp_t = &do_obj;

bool_t 
    udp_open
        (udp* self, obj_trait* arg)                                 {
            if (trait_of(self) != udp_t) return false_t; int af = -1;
            if (arg == v4_t) af = AF_INET  ;
            if (arg == v6_t) af = AF_INET6 ;
            if (af == -1)    return false_t;

            if (self->udp != INVALID_SOCKET) return true_t;
            self->udp = WSASocket                         (
                af                ,
                SOCK_DGRAM        ,
                IPPROTO_UDP       ,
                0                 ,
                0                 ,
                WSA_FLAG_OVERLAPPED
            );

            if (self->udp == INVALID_SOCKET) return false_t;
            self->ioc = CreateIoCompletionPort             (
                self->udp     ,
                self->run->hnd,
                self->run     ,
                0
            );

            if (!self->ioc)               {
                closesocket(self->udp)    ;
                self->udp = INVALID_SOCKET;
                return false_t;
            }
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
            closesocket (self->udp)   ;
            self->udp = INVALID_SOCKET;
}

fut*
    udp_send
        (udp* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != udp_t) return null_t;
            if (!buf)                    return null_t;
            if (!len)                    return null_t;
            

            io_res *ret = make (io_res) from (1, self->run); if (trait_of(ret) != io_res_t) return 0;
			WSABUF  snd = { .buf = buf, .len = len };
			i32_t   res = WSASend                   (
                self->udp ,
                &snd      ,
                1         ,
                0         ,
                self->flag,
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
    udp_send_to
        (udp* self, u8_t* buf, u64_t len, end* end)   {
            if (trait_of(self) != udp_t) return null_t;
            if (trait_of(end)  != end_t) return null_t;
            if (!buf)                    return null_t;
            if (!len)                    return null_t;

            io_res *ret = make (io_res) from (1, self->run); if (trait_of(ret) != io_res_t) return 0; 
			WSABUF  snd = { .buf = buf, .len = len };
			i32_t   res = WSASendTo                 (
                self->udp,
                &snd     ,
                1        ,
                0        ,
                0        ,
                &end->all,
                end->len ,
                &ret->res,
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
        (udp* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != udp_t) return null_t;
			if (!buf)                    return null_t;
            if (!len)                    return null_t;

            io_res *ret = make (io_res) from (1, self->run); if (trait_of(ret) != io_res_t) return 0;
			WSABUF  rcv = { .buf = buf, .len = len };
			i32_t   res = WSARecv                   (
                self->udp  ,
                &rcv       ,
                1          ,
                0          ,
                &self->flag,
                &ret->res  ,
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
        (udp* self, u8_t* buf, u64_t len, end* end)   {
            if (trait_of(self) != udp_t) return null_t;
            if (trait_of(end)  != end_t) return null_t;
            if (!buf)                    return null_t;
            if (!len)                    return null_t;

			io_res *ret = make (io_res) from (1, self->run); if (trait_of(ret) != io_res_t) return 0;
			WSABUF  rcv = { .buf = buf, .len = len };
			i32_t   res = WSARecvFrom               (
                self->udp,
                &rcv     ,
                1        ,
                0        ,
                0        ,
                &end->all,
                end->len ,
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