#include <tcp.h>
#include <tcp_acpt.h>
#include <run.h>
#include <async.h>

#include <stdio.h>
#include <Windows.h>

run()                                                     {
    v4  *tcp_addr = make(v4_t)  from(2, "127.0.0.1", 6500);
    tcp *tcp      = make(tcp_t) from(0)                   ;
    if (!tcp) return 0;

    u8_t* buf  = mem_new (0, 64)        ;
    task  conn = tcp_conn(tcp, tcp_addr);
    if  (!conn)
        return 1;

    mem_set(buf, 0x00, 64);
    await  (conn)         ;
    printf ("Connected\n");
    printf ("Received %s (%d Bytes).\n", buf, await(tcp_recv(tcp, buf, 64)));
}