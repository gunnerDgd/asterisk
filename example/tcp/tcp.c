#include <io.h>
#include <net.h>
#include <core.h>

#include <stdio.h>

use         (
    dep(net)
    dep(io)
)

run_async()                                                  {
    end  *end_1  = make (end)  from (2, make_v4_int(0), 6500);
    acpt *acpt_1 = make (acpt) from (0);

    if (!acpt_open (acpt_1, end_1, tcp_t))   {
        printf ("Faiiled to Open Acceptor\n");
        return 1;
    }

    while(true_t)                         {
        tcp *cli = await(acpt_fut(acpt_1));
        if (!cli)                                  {
            printf("Failed to Accept Connection\n");
            return 0;
        }

        printf("Accepted\n");
        printf("Sent %d Bytes\n", await(tcp_send(cli, "Hello World\n", 12)));

        del(cli);
    }

    del(acpt_1);
    del(end_1) ;
}