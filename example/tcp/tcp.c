#include "tcp.h"
#include "tcp_acpt.h"
#include "v4.h"

#include "io_sched.h"
#include "box.h"

#include <stdio.h>

io_sched_main(par_task, par)                                         {
    v4       *acpt_addr = make(v4_t)       from(2, "127.0.0.1", 6500);
    tcp_acpt *acpt      = make(tcp_acpt_t) from(1, par)              ;
    box      *buf       = make(box_t)      from(1, 64)               ;
    if (!buf)    {
        del(acpt);
        return  0;
    }

    ptr buf_ptr = box_ptr(buf, 0);
    ptr_write(buf_ptr, "Hello World\n", 12);

    if (!tcp_acpt_conn(acpt, acpt_addr))
        return false_t;
    printf("Start Accepting\n");
    while(true_t)                          {
        task *cli_acpt = tcp_acpt_run(acpt);
        tcp  *cli      = await(cli_acpt)   ;
        if  (!cli)                                 {
            printf("Failed to Create Connection\n");
            continue;
        }

        del(cli_acpt);
        printf("Accepted\n");

        task* cli_send = tcp_send(cli, buf_ptr, 12);
        printf("Sent %d Bytes\n", await_as(cli_send, u64_t));

        del(cli)     ;
        del(cli_send);
    }

    del(acpt_addr);
    del(acpt)     ;
    del(buf)      ;
    return 0;
}