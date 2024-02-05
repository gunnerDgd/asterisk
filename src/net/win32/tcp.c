#include "tcp.h"

#include "v4.h"
#include "v6.h"

obj_trait tcp_trait	    = {
	.on_new	  = &tcp_new  ,
	.on_clone = &tcp_clone,
	.on_ref   =		     0,
	.on_del	  = &tcp_del  ,
	.size	  = sizeof(tcp)
};

obj_trait* tcp_t = &tcp_trait;

bool_t 
	tcp_new
		(tcp* par_tcp, u32_t par_count, va_list par)							  {
			io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
			if (!sched)								sched = this_io_sched()		  ;
			if (!sched)									return false_t;
			if (trait_of(sched) != io_sched_t)			return false_t;
			if (!make_at(&par_tcp->end, end_t) from(0)) return false_t;
			par_tcp->tcp_io = 0				;
			par_tcp->tcp    = INVALID_SOCKET;
			par_tcp->sched  = ref(sched)    ;
			par_tcp->flag   = 0		        ;
			return true_t;
}

bool_t 
	tcp_clone
		(tcp* par, tcp* par_clone) {
			return false_t;
}

void
	tcp_del
		(tcp* par)				   {
			closesocket(par->tcp)  ;
			del		   (par->sched);
			
}

bool_t 
	tcp_open
		(tcp* par, obj_trait* par_af)											{
			if (!par)								 return false_t; int af = -1;
			if (trait_of(par) != tcp_t)				 return false_t;
			if (par->tcp != INVALID_SOCKET)			 return false_t;
			if (!make_at(&par->end, end_t) from (0)) return false_t;
			if (par_af == v4_t) af = AF_INET  ;
			if (par_af == v6_t) af = AF_INET6 ;
			if (af == -1)		return false_t;
			par->tcp = WSASocket			  (
				af				  ,
				SOCK_STREAM		  ,
				IPPROTO_TCP		  ,
				0				  ,
				0				  ,
				WSA_FLAG_OVERLAPPED
			);

			if (par->tcp == INVALID_SOCKET) return false_t;
			par->tcp_io = CreateIoCompletionPort		  (
				par->tcp	   ,
				par->sched->hnd,
				par->sched	   , 
				0
			);

			if (!par->tcp_io)		     {
				closesocket(par->tcp)    ;
				par->tcp = INVALID_SOCKET;
				return false_t;
			}
			return true_t;
}

bool_t
	tcp_conn_do
		(tcp* par)																						     {
			if (trait_of(par) != tcp_t)    goto conn_failed; io_res* ret = make(io_res_t) from(1, par->sched);
			if (trait_of(ret) != io_res_t) goto conn_failed;
			bool_t  res = ConnectEx						   (
				par->tcp	 ,
				&par->end.all,
				par->end.len ,
				0			 ,
				0			 ,
				0			 ,
				&ret->res
			);

			if (!res && WSAGetLastError() != ERROR_IO_PENDING) {
				tcp_close(par);
				del	     (ret);
				return false_t;
			}

			fut*     fut = io_res_fut(ret);
			await   (fut);
			del     (fut);
			del     (ret);
			return true_t;
	conn_failed:
			tcp_close(par);
			return false_t;
}

fut*
	tcp_conn
		(tcp* par, end* par_end)						 {
			if (trait_of(par_end) != end_t)		 return 0;
			if (trait_of(par)     != tcp_t)	     return 0;
			if (!tcp_open(par, end_af(par_end))) return 0; 
			
			par->end.af = par_end->af;
			if (bind(par->tcp, &par->end.all, par->end.len)) {
				tcp_close(par);
				return 0;
			}

			par->end.all = par_end->all;
			par->end.len = par_end->len;
			return async(tcp_conn_do, par);
}

void 
	tcp_close
		(tcp* par)							  {
			if (!par)					return;
			if (trait_of(par) != tcp_t) return;
			closesocket(par->tcp)		;
			del		   (&par->end)		;
			par->tcp_io = 0				;
			par->tcp    = INVALID_SOCKET;
}

fut*
	tcp_send
		(tcp* par, u8_t* par_buf, u64_t par_len) {
			if (!par_len)				return 0;
			if (!par_buf)				return 0;
			if (!par)					return 0;
			if (trait_of(par) != tcp_t) return 0; 
			
			io_res* ret = make(io_res_t) from(1, par->sched) ; if (trait_of(ret) != io_res_t) return 0;
			WSABUF  buf = { .buf = par_buf, .len = par_len  };
			i32_t   res = WSASend						     (
				par->tcp ,
				&buf	 ,
				1		 ,
				0		 ,
				par->flag,
				&ret->res,
				0
			);

			fut* fut = io_res_fut(ret);
			del (ret);
			if  (res && WSAGetLastError() != ERROR_IO_PENDING) {
				 ret->stat = fut_err;
				 return fut			;
			}
			return fut ;
}

fut*
	tcp_recv
		(tcp* par, u8_t* par_buf, u64_t par_len)   {
			if (!par_len)				   return 0;
			if (!par_buf)				   return 0;
			if (!par)					   return 0;
			if (trait_of(par) != tcp_t)    return 0;

			io_res *ret = make (io_res_t) from (1, par->sched); if (trait_of(ret) != io_res_t) return 0;
			WSABUF buf  = { .buf = par_buf, .len = par_len }  ;
			i32_t  res  = WSARecv							  (
				par->tcp  ,
				&buf	  ,
				1		  ,
				0		  ,
				&par->flag,
				&ret->res ,
				0
			);

			fut* fut = io_res_fut(ret);
			del (ret);
			if  (res && WSAGetLastError() != ERROR_IO_PENDING) {
			 	 ret->stat = fut_err;
				 return fut;
			}
			return fut ;
}