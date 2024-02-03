#include <asterisk/core.h>
#include <asterisk/net.h>

#include <stdio.h>

void async_main()                                                   {
    end *end = make(end_t) from (2, v4_from_cstr("127.0.0.1"), 6500);
    tcp *tcp = make(tcp_t) from (0);
    if (!tcp) return;
    if (!end) return;

    u8_t* buf  = new (u8_t[64]);
    if  (!await(tcp_conn(tcp, end))) return;

    mem_set(buf, 0x00, 64);
    printf ("Connected\n");
    printf ("Received %s (%d Bytes).\n", buf, await(tcp_recv(tcp, buf, 64)));

    drop(buf);
    del (tcp);
    del (end);
}