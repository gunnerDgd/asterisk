#include <tcp.h>
#include <tcp_acpt.h>
#include <run.h>
#include <async.h>

#include <stdio.h>

run()                                                                {
    v4       *acpt_addr = make(v4_t)       from(2, "127.0.0.1", 6500);
    tcp_acpt *acpt      = make(tcp_acpt_t) from(0)                   ;

    if (!tcp_acpt_conn(acpt, acpt_addr))     {
        printf("Failed to Create Connection");
        return false_t;
    }
    while(true_t)                           {
        tcp* cli = await(tcp_acpt_run(acpt));
        if (!cli)                                  {
            printf("Failed to Accept Connection\n");
            continue;
        }

        printf("Accepted\n");
        printf("Sent %d Bytes\n", await(tcp_send(cli, "Hello World\n", 12)));

        del(cli);
    }

    del(acpt_addr);
    del(acpt)     ;
    return 0;
}