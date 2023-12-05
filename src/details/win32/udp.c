#include "udp.h"

#include "io_task.h"
#include "io_sched.h"

#include "lib.h"
#include "curr.h"
#include "thd.h"

obj_trait __udp_trait      = {
    .on_new   = &__udp_new   ,
    .on_clone = &__udp_clone ,
    .on_ref   =            0 ,
    .on_del   = &__udp_del   ,
    .size     = sizeof(__udp)
};

bool_t 
    __udp_new
        (__udp* par_udp, u32_t par_count, va_list par) {
            par_udp->io_sched = (par_count == 1) ? ref(va_arg(par, __io_sched*)) : ref(&curr_thd->io_sched);
            if (!par_udp->io_sched)
                return false_t;

            par_udp->udp = WSASocketW (
                AF_INET            , 
                SOCK_DGRAM         , 
                IPPROTO_UDP        ,
                0                  , 
                0                  ,
                WSA_FLAG_OVERLAPPED
            );

            if (par_udp->udp == INVALID_SOCKET) return false_t;
            par_udp->udp_io_sched = CreateIoCompletionPort (
                par_udp->udp               ,
                par_udp->io_sched->io_sched, 
                par_udp->io_sched          ,
                0
            );

            if (!par_udp->udp_io_sched)      {
                closesocket(par_udp->udp)    ;
                par_udp->udp = INVALID_SOCKET;
                
                return false_t;
            }

            return true_t;
}

bool_t 
    __udp_clone
        (__udp* par, __udp* par_clone) {
            return false_t;
}

void   
    __udp_del
        (__udp* par)        {
            __udp_close(par);
            return;
}

bool_t 
    __udp_conn
        (__udp* par, __v4* par_v4)                                        {
            if (bind(par->udp, &par_v4->host, sizeof(struct sockaddr_in)))
                return false_t;

            return true_t;
}

void   
    __udp_close
        (__udp* par)                  {
            closesocket(par->udp)     ;
            del        (par->io_sched);

            par->udp = INVALID_SOCKET;
}

struct __io_task*
    __udp_send
        (__udp* par, u8_t* par_buf, u64_t par_len)             {
            __io_task* ret = __io_sched_dispatch(par->io_sched);
			if (!ret) return false_t;

            ret->state     = __io_task_state_pend              ;
			WSABUF res_buf = { .buf = par_buf, .len = par_len };
			i32_t  res     = WSASend (
                par->udp     ,
                &res_buf     ,
                1            ,
                0            ,
                0            ,
                &ret->io_task,
                0
            );

			if (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                list_push_back(&par->io_sched->io_task, ret);
				return 0;
			}

			return ret;
}

struct __io_task*
    __udp_send_to
        (__udp* par, u8_t* par_buf, u64_t par_len, __v4* par_v4) {
			__io_task* ret = __io_sched_dispatch(par->io_sched);
			if (!ret) return false_t;

            ret->state     = __io_task_state_pend              ;
			WSABUF res_buf = { .buf = par_buf, .len = par_len };
			i32_t  res     = WSASendTo    (
                par->udp                  ,
                &res_buf                  ,
                1                         ,
                0                         ,
                0                         ,
                &par_v4->host             ,
                sizeof(struct sockaddr_in),
                &ret->io_task             ,
                0
            );

			if (res && WSAGetLastError() != ERROR_IO_PENDING) {
                ret->state = __io_task_state_none           ;
                list_push_back(&par->io_sched->io_task, ret);
				return 0;
			}

			return ret;
}

struct __io_task*
    __udp_recv
        (__udp* par, u8_t* par_buf, u64_t par_len)             {
			__io_task* ret = __io_sched_dispatch(par->io_sched);
			if (!ret) return false_t;

            ret->state      = __io_task_state_pend;
            par->flag       = 0                   ;
			WSABUF res_buf  = { .buf = par_buf, .len = par_len };
			i32_t  res      = WSARecv (
                par->udp     ,
                &res_buf     ,
                1            ,
                0            ,
                &par->flag   ,
                &ret->io_task,
                0
            );

			if (res && WSAGetLastError() != ERROR_IO_PENDING) {
                ret->state = __io_task_state_none           ;
                list_push_back(&par->io_sched->io_task, ret);
				return 0;
			}

			return ret;
}

struct __io_task*
    __udp_recv_from
        (__udp* par, u8_t* par_buf, u64_t par_len, __v4* par_v4) {
			__io_task* ret = __io_sched_dispatch(par->io_sched);
			if (!ret) return false_t;

            ret   ->state = __io_task_state_pend      ;
            par_v4->size  = sizeof(struct sockaddr_in);
            par   ->flag  = 0                         ;

			WSABUF res_buf  = { .buf = par_buf, .len = par_len };
			i32_t  res      = WSARecvFrom (
                par->udp        ,
                &res_buf        ,
                1               ,
                0               ,
                &par->flag      ,
                &par_v4->host   ,
                &par_v4->size   ,
                &ret->io_task   ,
                0
            );

			if (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                ret->state = __io_task_state_none           ;
                list_push_back(&par->io_sched->io_task, ret);
				return 0;
			}

			return ret;
}