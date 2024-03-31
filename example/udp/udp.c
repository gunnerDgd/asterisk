#include <io.h>
#include <net.h>
#include <core.h>
#include <run.h>

#include <stdio.h>

run_async()                                                           {
    io_sched *sched_1 = make (io_sched) from (0)                      ;
    udp      *udp_1   = make (udp)      from (1, sched_1)             ;
    end      *end_1   = make (end)      from (2, make_v4_int(0), 6500);
    u8_t     *buf     = new  (u8_t[64]);
    mem_set(buf, 0x00, 64);
    if (!udp_conn(udp_1, end_1)) {
        drop(buf)  ;
        del (end_1);
        del (udp_1);

        return;
    }

    u64_t recv_size = await(udp_recv(udp_1, buf, 64));
    printf("Received %s (%d Bytes)\n", buf, recv_size);

    drop(buf);
    del (udp_1);
    del (end_1);
}