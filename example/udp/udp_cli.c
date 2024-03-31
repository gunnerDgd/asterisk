#include <io.h>
#include <net.h>
#include <core.h>
#include <run.h>

#include <stdio.h>
#include <stdlib.h>

run_async()                                                                      {
    io_sched *sched_1 = make (io_sched) from (0)                                 ;
    udp      *udp_1   = make (udp)      from (1, sched_1)                        ;
    end      *end_1   = make (end)      from (2, make_v4_cstr("127.0.0.1"), 6500);
    udp_open(udp_1, v4_t);
    u64_t send_size = await(udp_send_to(udp_1, "Hello World\n", 12, end_1));
    printf("Sent %d Bytes\n", send_size);
    del(udp_1);
    del(end_1);
    return;
}