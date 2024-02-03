#include <asterisk/core.h>
#include <asterisk/net.h>

#include <stdio.h>

void async_main()                                                  {
    end      *end = make(end_t)      from (2, v4_from_int(0), 6500);
    tcp_acpt *acp = make(tcp_acpt_t) from (0);

    if (!tcp_acpt_conn(acp, end))            {
        printf("Failed to Create Connection");
        return;
    }
    while(true_t)                          {
        tcp *cli = await(tcp_acpt_run(acp));
        if (!cli)                                  {
            printf("Failed to Accept Connection\n");
            continue;
        }

        printf("Accepted\n");
        printf("Sent %d Bytes\n", await(tcp_send(cli, "Hello World\n", 12)));

        del(cli);
    }

    del(end);
    del(acp);
    return;
}