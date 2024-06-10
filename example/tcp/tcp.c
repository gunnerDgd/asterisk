#include <io.h>
#include <net.h>
#include <core.h>

use         (
    dep(net)
    dep(io)
)

run_async()                                                  {
    end  *end_1  = make (end)  from (2, make_v4_int(0), 6500);
    acpt *acpt_1 = make (acpt) from (0);

    if (!acpt_open (acpt_1, end_1, tcp_t))  {
        println ("Faiiled to Open Acceptor");
        return 1;
    }

    while(true_t)                         {
        tcp *cli = await(acpt_fut(acpt_1));
        if (!cli)                                 {
            println("Failed to Accept Connection");
            return 0;
        }

        println("Accepted");
        println("Sent %d Bytes", await(tcp_send(cli, "Hello World\n", 12)));

        del(cli);
    }

    del(acpt_1);
    del(end_1) ;
}