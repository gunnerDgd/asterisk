#include "tcp.h"
#include "tcp_acpt.h"

#include "io_sched.h"


void* async_main(task* par_task, io_sched* par) {
    tcp_acpt* acpt    = make(tcp_acpt_t) from(1, par);
    mem       buf     = mem_init(0, 64);
    ptr       buf_ptr = mem_ptr (buf)  ;

    ptr_write(buf_ptr, "Hello World\n", 12);
    while(true_t)                          {
        tcp* cli = await(tcp_acpt_run(par));
        if (!cli) continue;

        tcp_send(cli, buf_ptr, 12);
    }
}

int main() {
    io_sched *sched      = make(io_sched_t) from (0)                   ;
    task     *sched_main = make(task_t)     from (2, async_main, sched);

    io_sched_dispatch(sched, sched_main);
    io_sched_run     (sched);
}