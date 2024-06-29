#include <io.h>
#include <net.h>
#include <core.h>

use         (
    dep(net)
    dep(io)
)

run_async()                       {
    v4  *v4_1  = new_v4("0.0.0.0");
    end *end_1 = make (end) from (2, v4_1, 6500);
    acp *acp_1 = new_acp(end_1, tcp_t);

    while(true_t)                       {
        tcp *cli = await(acp_fut(acp_1));
        if (!cli)                                 {
            println("Failed to Accept Connection");
            return 0;
        }

        println("Accepted");
        println("Sent %d Bytes", await(tcp_send(cli, "Hello World\n", 12)));

        del(cli);
    }

    del(acp_1);
    del(end_1);
}