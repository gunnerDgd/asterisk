#include "tcp.h"
#include "tcp_acpt.h"

#include "io_sched.h"
#include "box.h"

#include <stdio.h>

io_sched_main(par_task, par)                      {
    tcp_acpt *acpt = make(tcp_acpt_t) from(1, par);
    box      *buf  = make(box_t)      from(1, 64) ;
    if (!buf)    {
        del(acpt);
        return  0;
    }

    ptr buf_ptr = box_ptr(buf, 0);
    ptr_write(buf_ptr, "Hello World\n", 12);

    if (!tcp_acpt_conn(acpt, "127.0.0.1", 6500))
        return false_t;
    printf("Start Accepting\n");
    while(true_t)                           {
        tcp* cli = await(tcp_acpt_run(acpt));
        if (!cli)                                  {
            printf("Failed to Create Connection\n");
            continue;
        }
        printf("Accepted\n");
        printf("Sent %d Bytes\n", await_as(tcp_send(cli, buf_ptr, 12), u64_t));
    }

    printf("Completed\n");
    return 0;
}