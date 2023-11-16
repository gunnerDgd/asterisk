#include "udp.h"
#include "io_res.h"

obj_trait __udp_trait                  = {
    .init          = &__udp_init         ,
    .init_as_clone = &__udp_init_as_clone,
    .init_as_ref   =                    0,
    .deinit        = &__udp_deinit       ,
    .size          = &__udp_size         ,
    .name          =                    0
};

bool_t 
    __udp_init
        (__udp* par_udp, u32_t par_count, va_list par) {
            par_udp->io_sched = ref(va_arg(par, __io_sched*));
            if (!par_udp->io_sched)
                return false_t;

            par_udp->udp      = INVALID_SOCKET;
            par_udp->udp_iocp = 0             ;

            return true_t;
}

bool_t 
    __udp_init_as_clone
        (__udp* par, __udp* par_clone) {
            return false_t;
}

void   
    __udp_deinit
        (__udp* par) {
            __udp_close(par);
            return;
}

u64_t
    __udp_size()            {
        return sizeof(__udp);
}

bool_t 
    __udp_conn
        (__udp* par, __v4* par_v4)         {
            if(par->udp == INVALID_SOCKET) {
                par->udp = WSASocketW (AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, 0);
                if (par->udp == INVALID_SOCKET)
                   return false_t;
            }

            par->udp_iocp = CreateIoCompletionPort(par->udp, par->udp_iocp, par->udp_iocp, 0);
            if (!par->udp_iocp)          {
                closesocket(par->udp)    ;
                par->udp = INVALID_SOCKET;
                
                return false_t;
            }

            if(bind(par->udp, &par_v4->v4, sizeof(struct sockaddr_in))) {
                closesocket(par->udp)     ;
                CloseHandle(par->udp_iocp);

                par->udp      = INVALID_SOCKET;
                par->udp_iocp = 0;

                return false_t;
            }

            return true_t;
}

void   
    __udp_close
        (__udp* par)                  {
            closesocket(par->udp)     ;
            CloseHandle(par->udp_iocp);

            par->udp      = INVALID_SOCKET;
            par->udp_iocp =              0;
}

task* 
    __udp_send
        (__udp* par, ptr par_buf, u64_t par_len) {
            if(par_len > ptr_size(par_buf))
				return 0;

			__io_res* res = make(&__io_res_trait) from(1, par->io_sched);
			if (!res)
				return false_t;

			WSABUF ret_buf			 = { 
				.buf = ptr_raw(par_buf), 
				.len = par_len 
			};

			i32_t ret = WSASend(par->udp, &ret_buf, 1, 0, 0, &res->hnd, 0);
			if   (ret && WSAGetLastError() != ERROR_IO_PENDING) {
				del(res);
				return 0;
			}

			return res->task;
}

task* 
    __udp_send_to
        (__udp* par, ptr par_buf, u64_t par_len, __v4* par_v4) {
            if(par_len > ptr_size(par_buf))
				return 0;

			__io_res* res = make(&__io_res_trait) from(1, par->io_sched);
			if (!res)
				return false_t;

			WSABUF ret_buf			 = { 
				.buf = ptr_raw(par_buf), 
				.len = par_len 
			};

			i32_t ret = WSASendTo         (
                par->udp                  ,
                &ret_buf                  ,
                1                         ,
                0                         ,
                0                         ,
                &par_v4->v4               ,
                sizeof(struct sockaddr_in), 
                &res->hnd                 , 
                0
            );

			if (ret && WSAGetLastError() != ERROR_IO_PENDING) {
				del(res);
				return 0;
			}

			return res->task;
}

task* 
    __udp_recv
        (__udp* par, ptr par_buf, u64_t par_len) {
            if(par_len > ptr_size(par_buf))
				return 0;

			__io_res* res = make(&__io_res_trait) from(1, par->io_sched);
			if (!res)
				return false_t;

            u32_t  ret_flag          = 0;
			WSABUF ret_buf           = { 
				.buf = ptr_raw(par_buf),
				.len = par_len 
			};

			i32_t ret = WSARecv(par->udp, &ret_buf, 1, 0, &ret_flag, &res->hnd, 0);
			if   (ret && WSAGetLastError() != ERROR_IO_PENDING) {
				del(res);
				return 0;
			}

			return res->task;
}

task* 
    __udp_recv_from
        (__udp* par, ptr par_buf, u64_t par_len, __v4* par_v4) {
            if(par_len > ptr_size(par_buf))
				return 0;

			__io_res* res = make(&__io_res_trait) from(1, par->io_sched);
			if (!res)
				return false_t;

            u32_t  ret_flag          = 0;
			WSABUF ret_buf           = {
				.buf = ptr_raw(par_buf), 
				.len = par_len 
			};

			i32_t ret = WSARecvFrom       (
                par->udp                  ,
                &ret_buf                  ,
                1                         ,
                0                         ,
                &ret_flag                 ,
                &par_v4->v4               ,
                sizeof(struct sockaddr_in),
                &res->hnd                 , 
                0
            );

			if (ret && WSAGetLastError() != ERROR_IO_PENDING) {
				del(res);
				return 0;
			}

			return res->task;
}