#include "tcp.h"

#include "../../io.h"
#include "net.h"
#include "v4.h"
#include "v6.h"

obj_trait tcp_trait	= make_trait (
	tcp_new	   ,
	tcp_clone  ,
	null_t	   ,
	tcp_del	   ,
	sizeof(tcp),
	null_t
);

obj_trait* tcp_t = &tcp_trait;

bool_t 
	tcp_new
		(tcp* self, u32_t count, va_list arg)									   {
			io_sched* sched = null_t; if (count > 0) sched = va_arg(arg, io_sched*);
			if (trait_of(sched) != io_sched_t) sched = this_io_sched();
			if (trait_of(sched) != io_sched_t) return false_t;
			self->ioc   = null_t		;
			self->tcp   = INVALID_SOCKET;
			self->sched = ref(sched)    ;
			self->flag  = 0			    ;
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
		(tcp* self, obj_trait* pro)							        {
			if (trait_of(self) != tcp_t) return false_t; int af = -1;
			if (pro == v4_t) af = AF_INET  ;
			if (pro == v6_t) af = AF_INET6 ;
			if (af == -1)	 return false_t;

			if (self->tcp != INVALID_SOCKET) return false_t;
			self->tcp = WSASocket						   (
				af				  ,
				SOCK_STREAM		  ,
				IPPROTO_TCP		  ,
				0				  ,
				0				  ,
				WSA_FLAG_OVERLAPPED
			);

			if (self->tcp == INVALID_SOCKET) return false_t;
			self->ioc = CreateIoCompletionPort		       (
				self->tcp	    ,
				self->sched->hnd,
				self->sched	    ,
				0
			);

			if (!self->ioc)		          {
				closesocket(self->tcp)    ;
				self->tcp = INVALID_SOCKET;
				return false_t;
			}
			return true_t;
}

fut*
	tcp_conn
		(tcp* self, end* arg)							   {
			if (trait_of(self) != tcp_t)	  return null_t;
			if (trait_of(arg)  != end_t)	  return null_t;
			if (!tcp_open(self, end_af(arg))) return null_t;
			SOCKADDR_IN6 end;
			
			mem_set(&end, 0x00, sizeof(end));
			end.sin6_family = arg->af;
			if (bind(self->tcp, &end, arg->len)) {
				tcp_close(self);
				return 0;
			}

			io_res *ret = make(io_res) from (1, self->sched); if (trait_of(ret) != io_res_t) return 0;
			bool_t  res = ConnectEx						    (
				self->tcp,
				&arg->all,
				arg->len ,
				0		 ,
				0		 ,
				0		 ,
				&ret->res
			);

			fut* fut = io_res_fut(ret);
			ret->ret = self;

			del   (ret);
			return fut;
}

void 
	tcp_close
		(tcp* par)				 {
			closesocket(par->tcp);
			par->ioc =  0;
			par->tcp    = -1;
}

fut*
	tcp_send
		(tcp* self, u8_t* buf, u64_t len)			  {
			if (trait_of(self) != tcp_t) return null_t;
			if (!len)					 return null_t;
			if (!buf)					 return null_t;
			WSABUF  iob								= {
				.buf = buf,
				.len = len 
			};
			
			io_res* ret = make (io_res) from (1, self->sched); if (trait_of(ret) != io_res_t) return 0;
			i32_t   res = WSASend						     (
				self->tcp ,
				&iob      ,
				1		  ,
				0		  ,
				self->flag,
				&ret->res ,
				0
			);

			fut* fut = io_res_fut(ret);
			del (ret);
			if  (res && WSAGetLastError() != ERROR_IO_PENDING) {
				 ret->stat = fut_err		  ;
				 ret->ret  = WSAGetLastError();
				 return fut;
			}

			return fut;
}

fut*
	tcp_recv
		(tcp* self, u8_t* buf, u64_t len)			  {
			if (trait_of(self) != tcp_t) return null_t;
			if (!len)				     return null_t;
			if (!buf)					 return null_t;
			WSABUF iob						 	    = { 
				.buf = buf, 
				.len = len 
			};

			io_res *ret = make (io_res) from (1, self->sched); if (trait_of(ret) != io_res_t) return 0;
			i32_t   res = WSARecv							 (
				self->tcp  ,
				&iob       ,
				1		   ,
				0		   ,
				&self->flag,
				&ret->res  ,
				0
			);

			fut* fut = io_res_fut(ret);
			del (ret);
			if  (res && WSAGetLastError() != ERROR_IO_PENDING) {
			 	 ret->stat = fut_err		  ;
				 ret->ret  = WSAGetLastError();
				 return fut;
			}

			return fut;
}