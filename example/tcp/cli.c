#include <io.h>
#include <net.h>
#include <core.h>

#include <stdio.h>

void run_async()                                                                    {
    io_sched *io_sched_1 = make (io_sched) from (0)                                 ;
    end      *end_1      = make (end)      from (2, make_v4_cstr("127.0.0.1"), 6500);
    tcp      *tcp_1      = make (tcp)      from (1, io_sched_1);
    if (!tcp_1) return;
    if (!end_1) return;

    u8_t* buf  = new (u8_t[64]);
    if  (!await(tcp_conn(tcp_1, end_1))) return;

    mem_set(buf, 0x00, 64);
    printf ("Connected\n");
    printf ("Received %s (%d Bytes).\n", buf, await(tcp_recv(tcp_1, buf, 64)));

    drop(buf);
    del (io_sched_1);
    del (end_1);
    del (tcp_1);
}