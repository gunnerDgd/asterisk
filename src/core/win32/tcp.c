#include "tcp.h"
#include "io_res.h"
#include "this.h"

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
		(tcp* par_tcp, u32_t par_count, va_list par)					  {
			io_run* run = 0; if (par_count > 0) run = va_arg(par, io_run*);
			if (!run)					   return false_t;
			if (trait_of(run) != io_run_t) return false_t;
			par_tcp->tcp = WSASocket (
				AF_INET		      ,
				SOCK_STREAM		  , 
				0				  , 
				0				  ,
				0				  ,
				WSA_FLAG_OVERLAPPED
			);
			
			if (par_tcp->tcp == INVALID_SOCKET) return false_t;
			par_tcp->tcp_io = CreateIoCompletionPort		  (
				par_tcp->tcp,
				run->hnd    ,
				run			,
				0
			);

			if (!par_tcp->tcp_io)		 {
				closesocket(par_tcp->tcp);
				return false_t;
			}

			par_tcp->flag = 0		;
			par_tcp->run  = ref(run);
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
			del		   (par->run);
			closesocket(par->tcp)  ;
}

bool_t
	tcp_conn_v4_do
		(tcp* par)										  {
			io_res *ret = make(io_res_t) from(1, par->run);
			v4	   *v4  = par->addr						  ;
			if (!ret)					   return false_t;
			if (!v4)					   return false_t;
			if (trait_of(ret) != io_res_t) return false_t;
			if (trait_of(v4)  != v4_t)     return false_t;
			bool_t res = ConnectEx (
				par->tcp	  ,
				&v4->v4		  ,
				sizeof(v4->v4),
				0			  ,
				0			  ,
				0			  ,
				&ret->res
			);

			if (!res && WSAGetLastError() != ERROR_IO_PENDING)
				return false_t;

			fut*   fut = io_res_fut(ret);
			await (fut);
			del   (fut);
			del   (ret);
			return true_t;
}

fut*
	tcp_conn_v4
		(tcp* par, v4* par_v4)				           {
			if (!par_v4)					   return 0;
			if (!par)					       return 0;
			if (trait_of(par_v4) != v4_t)	   return 0;
			if (trait_of(par)    != tcp_t)	   return 0;

			if (par->tcp    == INVALID_SOCKET) return 0;
			if (par->tcp_io == NULL)           return 0;

			v4  *v4 = make (v4_t) from (0);
			if (!v4)									 goto conn_failed;
			if (trait_of(v4) != v4_t)					 goto conn_failed;
			if (bind(par->tcp, &v4->v4, sizeof(v4->v4))) goto conn_failed;
			if (!clone_at(v4, par_v4))					 goto conn_failed;
			par->addr = v4;

			return async(tcp_conn_v4_do, par);

	conn_failed:
			closesocket(par->tcp)		;
			par->tcp    = INVALID_SOCKET;
			par->tcp_io = 0			    ;

			return 0;
}

fut*
	tcp_conn
		(tcp* par, obj* par_addr)				{
			if (!par_addr)				return 0;
			if (!par)					return 0;
			if (trait_of(par) != tcp_t) return 0;

			if (trait_of(par_addr) == v4_t) return tcp_conn_v4(par, par_addr);
			return 0;
}

void 
	tcp_close
		(tcp* par)				 {
			closesocket(par->tcp); 
}

fut*
	tcp_send
		(tcp* par, u8_t* par_buf, u64_t par_len)       {
			if (!par_len)				       return 0;
			if (!par_buf)				       return 0;
			if (!par)					       return 0;
			
			if (trait_of(par) != tcp_t)		   return 0;
			if (par->tcp    == INVALID_SOCKET) return 0;
			if (par->tcp_io == NULL)           return 0;

			io_res *ret = make (io_res_t) from (1, par->run);
			if (!ret)					   return 0;
			if (trait_of(ret) != io_res_t) return 0;

			WSABUF buf = { .buf = par_buf, .len = par_len  } ;
			i32_t  res = WSASend (
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
			if (res && WSAGetLastError() != ERROR_IO_PENDING) {
				ret->stat = fut_err;
				return fut;
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

			io_res *ret = make (io_res_t) from (1, par->run);
			if (!ret)					   return 0;
			if (trait_of(ret) != io_res_t) return 0;

			WSABUF buf = { .buf = par_buf, .len = par_len };
			i32_t  res = WSARecv (
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
			if (res && WSAGetLastError() != ERROR_IO_PENDING) {
				ret->stat = fut_err;
				return fut;
			}
			return fut ;
}