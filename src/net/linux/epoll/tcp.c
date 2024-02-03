#include "tcp.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

obj_trait tcp_trait	    = {
	.on_new	  = &tcp_new  ,
	.on_clone = &tcp_clone,
	.on_ref   =		     0,
	.on_del	  = &tcp_del  ,
	.size	  = sizeof(tcp)
};

obj_trait* tcp_t = &tcp_trait;

i64_t
    tcp_do_conn
        (tcp* par, u8_t* par_buf, u64_t par_len, void* par_arg) {
            if (!par)                     return -1       ;
            if (trait_of(par) != tcp_t)   return -1       ;
            if (io_poll_hang(&par->poll)) goto   conn_err ;
            if (io_poll_err (&par->poll)) goto   conn_err ;
            if (!io_poll_out(&par->poll)) goto   conn_pend;

            io_poll_mask_out(&par->poll, false_t);
            errno = 0;
            return  1;
    conn_err :
            errno = EFAULT;
            return -1;
    conn_pend:
            errno = EAGAIN;
            return 0;
}

i64_t
    tcp_do_recv
        (tcp* par, u8_t* par_buf, u64_t par_len,  void* par_arg) {
            if (!par)                     return -1       ;
            if (trait_of(par) != tcp_t)   return -1       ;
            if (io_poll_hang(&par->poll)) goto   recv_err ;
            if (io_poll_err (&par->poll)) goto   recv_err ;
            if (!io_poll_in(&par->poll))  goto   recv_pend;
            i64_t ret = recv                              (
                par->tcp ,
                par_buf  ,
                par_len  ,
                par->flag
            );

            if (ret > 0) errno = 0;
            return ret;
    recv_err :
            errno = EFAULT;
            return -1;
    recv_pend:
            errno = EAGAIN;
            return 0;
}

i64_t
    tcp_do_send
        (tcp* par, u8_t* par_buf, u64_t par_len)         {
            if (!par)                     return 0       ;
            if (trait_of(par) != tcp_t)   return 0       ;
            if (io_poll_hang(&par->poll)) goto   send_err;
            if (io_poll_err (&par->poll)) goto   send_err;
            i64_t ret = send                             (
                par->tcp ,
                par_buf  ,
                par_len  ,
                par->flag
            );

            if (ret > 0) errno = 0;
            return ret;
    send_err :
            errno = EFAULT;
            return -1;
}

bool_t 
	tcp_new
		(tcp* par_tcp, u32_t par_count, va_list par)		     	              {
			io_sched* sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
			if (!sched)						        sched = this_io_sched();
			if (!sched)						   return false_t;
			if (trait_of(sched) != io_sched_t) return false_t;
			par_tcp->sched = ref(sched);
			return true_t;
}

bool_t 
	tcp_clone
		(tcp* par, tcp* par_clone) {
			return false_t;
}

void
	tcp_del
		(tcp* par)         {
            tcp_close(par) ;
			del(par->sched);
}

fut*
	tcp_conn
		(tcp* par, end* par_end)				    {
			if (!par_end)				    return 0;
			if (!par)					    return 0;
			if (trait_of(par_end) != end_t) return 0;
			if (trait_of(par)     != tcp_t) return 0;
            par->tcp = socket                       (
                par_end->end.type          ,
                SOCK_STREAM | SOCK_NONBLOCK,
                IPPROTO_TCP
            );

            if (par->tcp <= 0)                                                  return 0;
            if (!make_at(&par->poll, io_poll_t) from (2, par->sched, par->tcp)) return 0;
            io_res *ret = make (io_res_t) from (
                4          ,
                par        ,
                tcp_do_conn,
                0          ,
                1
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret); del (ret);
            if (trait_of(fut) != fut_t)    return 0;

            int res = connect(par->tcp, &par_end->end, par_end->len);
            if (!res)                {
                ret->stat = fut_ready;
                ret->ret  = 1        ;
                return fut;
            }

            if (res == -1 && errno == EINPROGRESS)  {
                io_poll_mask_out(&par->poll, true_t);
                errno  = 0;
                return fut;
            }

            del(fut);
            return 0;
}

void 
	tcp_close
		(tcp* par)                              {
		    if   (!par)                   return;
		    if   (trait_of(par) != tcp_t) return;
            close(par->tcp)  ;
            del  (&par->poll);
            par->tcp = 0;
}

fut*
	tcp_send
		(tcp* par, u8_t* par_buf, u64_t par_len) {
			if (!par_len)				return 0;
			if (!par_buf)				return 0;
			if (!par)					return 0;
			if (trait_of(par) != tcp_t)	return 0;
			if (!par->tcp)              return 0;
			io_res *ret = make (io_res_t) from  (
			    4          ,
			    par        ,
			    tcp_do_send,
			    par_buf    ,
			    par_len
			);

			if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret); del (ret);
			if (trait_of(fut) != fut_t)    return 0;
            return fut;
}

fut*
	tcp_recv
		(tcp* par, u8_t* par_buf, u64_t par_len) {
            if (!par_len)				return 0;
            if (!par_buf)				return 0;
            if (!par)					return 0;
            if (trait_of(par) != tcp_t)	return 0;
            if (!par->tcp)              return 0;
            io_res *ret = make (io_res_t) from  (
                4          ,
                par        ,
                tcp_do_recv,
                par_buf    ,
                par_len
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret); del (ret);
            if (trait_of(fut) != fut_t)    return 0;
            return fut;
}