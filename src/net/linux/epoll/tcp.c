#include "tcp.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

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
            if (!par)                          return -1;
            if (trait_of(par) != tcp_t)        return -1;
            if (io_dev_stat(&par->dev)->avail) return  1;
            if (io_dev_stat(&par->dev)->in)             {
                io_dev_stat(&par->dev)->avail = 1;
                io_dev_stat(&par->dev)->in    = 0;
                errno = 0;
                return  1;
            }

            errno = EAGAIN;
            return 0;
}

i64_t
    tcp_do_recv
        (tcp* par, u8_t* par_buf, u64_t par_len,  void* par_arg) {
            if (!par)                        return -1       ;
            if (trait_of(par) != tcp_t)      return -1       ;
            if (!io_dev_stat(&par->dev)->in) goto   recv_pend;
            i64_t ret = recv                                 (
                par->tcp ,
                par_buf  ,
                par_len  ,
                par->flag
            );

            if (ret > 0)         errno = 0;
            io_dev_stat(&par->dev)->in = 0;
            return ret;
    recv_pend:
            errno = EAGAIN;
            return 0;
}

i64_t
    tcp_do_send
        (tcp* par, u8_t* par_buf, u64_t par_len) {
            if (!par)                   return 0;
            if (trait_of(par) != tcp_t) return 0;
            i64_t ret = send                    (
                par->tcp ,
                par_buf  ,
                par_len  ,
                par->flag
            );

            if (ret != -1 && ret < par_len) errno = EAGAIN;
            io_dev_stat(&par->dev)->out = 0;
            return ret;
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
	tcp_conn_v4
		(tcp* par, v4* par_v4)				       {
			if (!par_v4)				   return 0;
			if (!par)					   return 0;
			if (trait_of(par_v4) != v4_t)  return 0;
			if (trait_of(par)    != tcp_t) return 0;
            par->tcp  = socket                     (
                AF_INET                    ,
                SOCK_STREAM | SOCK_NONBLOCK,
                IPPROTO_TCP
            );

            if (par->tcp <= 0)                                                return 0;
            if (!make_at(&par->dev, io_dev_t) from (2, par->sched, par->tcp)) return 0;
            io_res *ret = make (io_res_t) from (
                4          ,
                par        ,
                tcp_do_conn,
                0          ,
                1
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
            if (trait_of(fut) != fut_t)    return 0; del (ret);

            int res = connect(par->tcp, &par_v4->v4, sizeof(par_v4->v4));
            if (!res)                {
                ret->stat = fut_ready;
                ret->ret  = 1        ;
                return fut;
            }

            if (errno == EAGAIN)      return fut;
            if (errno == EINPROGRESS) return fut;
            del(fut);
            return 0;
}

fut*
	tcp_conn
		(tcp* par, obj* par_addr)				     {
			if (!par_addr)				     return 0;
			if (!par)					     return 0;
			if (trait_of(par)      != tcp_t) return 0;
			if (trait_of(par_addr) == v4_t)  return tcp_conn_v4(par, par_addr);
			return 0;
}

void 
	tcp_close
		(tcp* par)                            {
		    if (!par)                   return;
		    if (trait_of(par) != tcp_t) return;
		    epoll_ctl(par->sched->hnd, EPOLL_CTL_DEL, par->tcp, 0);
            close    (par->tcp);
}

fut*
	tcp_send
		(tcp* par, u8_t* par_buf, u64_t par_len) {
			if (!par_len)				return 0;
			if (!par_buf)				return 0;
			if (!par)					return 0;
			if (trait_of(par) != tcp_t)	return 0;
			io_res *ret = make (io_res_t) from (
			    4          ,
			    par        ,
			    tcp_do_send,
			    par_buf    ,
			    par_len
			);

			if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
			if (trait_of(fut) != fut_t)    return 0; del (ret);
            return fut;
}

fut*
	tcp_recv
		(tcp* par, u8_t* par_buf, u64_t par_len) {
            if (!par_len)				return 0;
            if (!par_buf)				return 0;
            if (!par)					return 0;
            if (trait_of(par) != tcp_t)	return 0;
            io_res *ret = make (io_res_t) from (
                4          ,
                par        ,
                tcp_do_recv,
                par_buf    ,
                par_len
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
            if (trait_of(fut) != fut_t)    return 0; del (ret);
            return fut;
}