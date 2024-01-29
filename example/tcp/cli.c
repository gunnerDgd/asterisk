#include <asterisk/tcp.h>
#include <asterisk/tcp_acpt.h>
#include <asterisk/this.h>

#include <asterisk/io_serv.h>
#include <asterisk/io_run.h>

#include <stdio.h>

void async_main()                                {
    io_run  *run  = make(io_run_t)  from (0)     ;
    io_serv *serv = make(io_serv_t) from (1, run);
    tcp     *tcp  = make(tcp_t)     from (1, run);
    v4      *addr = make(v4_t)      from (0);
    if (!addr) return 0;
    if (!tcp)  return 0;

    v4_from_cstr(addr, "127.0.0.1");
    v4_port     (addr, 6500)       ;

    u8_t* buf  = new (u8_t[64]);
    if  (!await(tcp_conn(tcp, addr))) return 1;

    mem_set(buf, 0x00, 64);
    printf ("Connected\n");
    printf ("Received %s (%d Bytes).\n", buf, await(tcp_recv(tcp, buf, 64)));

    drop(buf) ;
    del (run) ;
    del (serv);
    del (tcp) ;
    del (addr);
}