#include <io.h>
#include <net.h>
#include <core.h>

#include <stdio.h>

void run_async()                                                         {
    io_sched *io_sched_1 = make (io_sched) from (0)                      ;
    end      *end_1      = make (end)      from (2, make_v4_int(0), 6500);
    tcp_acpt *acpt_1     = make (tcp_acpt) from (2, io_sched_1, end_1)   ;

    while(true_t)                            {
        tcp *cli = await(tcp_acpt_run(acpt_1));
        if (!cli)                                  {
            printf("Failed to Accept Connection\n");
            return;
        }

        printf("Accepted\n");
        printf("Sent %d Bytes\n", await(tcp_send(cli, "Hello World\n", 12)));

        del(cli);
    }

    del(acpt_1);
    del(end_1) ;
}