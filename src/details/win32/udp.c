#include "udp.h"

#include "io_task.h"
#include "io_sched.h"

#include "lib.h"
#include "curr.h"
#include "thd.h"

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
        (udp* par_udp, u32_t par_count, va_list par)                                           {
            par_udp->io_sched = (par_count == 1) ? va_arg(par, io_sched*) : &curr_thd->io_sched;
            if (!par_udp->io_sched)                        return false_t;
            if (trait_of(par_udp->io_sched) != io_sched_t) return false_t;

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
    udp_clone
        (udp* par, udp* par_clone) {
            return false_t;
}

void   
    udp_del
        (udp* par)        {
            udp_close(par);
            return;
}

bool_t 
    udp_conn
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
        (udp* par)                    {
            closesocket(par->udp)     ;
            del        (par->io_sched);

            par->udp = INVALID_SOCKET;
}

struct io_task*
    udp_send
        (udp* par, u8_t* par_buf, u64_t par_len)             {
            io_task *ret = io_sched_dispatch(par->io_sched);
			if     (!ret) return false_t;

            ret->state     = io_task_exec                      ;
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
                ret->state = io_task_free                ;
                list_push_back(&par->io_sched->none, ret);

				return 0;
			}

			return ret;
}

struct io_task*
    udp_send_to
        (udp* par, u8_t* par_buf, u64_t par_len, v4* par_v4) {
			io_task *ret = io_sched_dispatch(par->io_sched);
			if     (!ret) return false_t;

            ret->state     = io_task_exec                      ;
			WSABUF res_buf = { .buf = par_buf, .len = par_len };
			i32_t  res     = WSASendTo    (
                par->udp                  ,
                &res_buf                  ,
                1                         ,
                0                         ,
                0                         ,
                &par_v4->v4               ,
                sizeof(struct sockaddr_in),
                &ret->io_task             ,
                0
            );

			if (res && WSAGetLastError() != ERROR_IO_PENDING) {
                ret->state = io_task_free                ;
                list_push_back(&par->io_sched->none, ret);
				return 0;
			}

			return ret;
}

struct io_task*
    udp_recv
        (udp* par, u8_t* par_buf, u64_t par_len)             {
			io_task *ret = io_sched_dispatch(par->io_sched);
			if     (!ret) return false_t;

            ret->state      = io_task_exec                      ;
            par->flag       = 0                                 ;
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
                ret->state = io_task_free                ;
                list_push_back(&par->io_sched->none, ret);
				return 0;
			}

			return ret;
}

struct io_task*
    udp_recv_from
        (udp* par, u8_t* par_buf, u64_t par_len, v4* par_v4) {
			io_task *ret = io_sched_dispatch(par->io_sched);
			if     (!ret) return false_t;

            ret   ->state = io_task_exec              ;
            par_v4->size  = sizeof(struct sockaddr_in);
            par   ->flag  = 0                         ;

			WSABUF res_buf  = { .buf = par_buf, .len = par_len };
			i32_t  res      = WSARecvFrom (
                par->udp        ,
                &res_buf        ,
                1               ,
                0               ,
                &par->flag      ,
                &par_v4->v4     ,
                &par_v4->size   ,
                &ret->io_task   ,
                0
            );

			if (ret && WSAGetLastError() != ERROR_IO_PENDING) {
                ret->state = io_task_free                ;
                list_push_back(&par->io_sched->none, ret);
				return 0;
			}

			return ret;
}