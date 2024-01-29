#include <asterisk/core.h>
#include <asterisk/net.h>

#include <stdio.h>

void async_main()                             {
    tcp_acpt* acpt = make(tcp_acpt_t) from (0);
    v4       *addr = make(v4_t)       from (0);

    v4_port(addr, 6500);
    if (!tcp_acpt_conn(acpt, addr))          {
        printf("Failed to Create Connection");
        return false_t;
    }
    while(true_t)                           {
        tcp *cli = await(tcp_acpt_run(acpt));
        if (!cli)                                  {
            printf("Failed to Accept Connection\n");
            continue;
        }

        printf("Accepted\n");
        printf("Sent %d Bytes\n", await(tcp_send(cli, "Hello World\n", 12)));

        del(cli);
    }

    del(addr);
    del(acpt);
    return 0;
}