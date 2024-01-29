#include <asterisk/udp.h>
#include <asterisk/this.h>
#include <asterisk/v4.h>

#include <asterisk/io_serv.h>
#include <asterisk/io_run.h>

#include <stdio.h>
#include <stdlib.h>

void async_main()                                {
    io_run  *run  = make(io_run_t)  from (0)     ;
    io_serv *serv = make(io_serv_t) from (1, run);
    v4      *addr = make(v4_t)      from (0)     ;
    udp     *udp  = make(udp_t)     from (1, run);
    v4_from_cstr(addr, "127.0.0.1");
    v4_port     (addr, 6500);
    u64_t send_size = await(udp_send_to(udp, "Hello World\n", 12, addr));
    printf("Sent %d Bytes\n", send_size);
    del(serv);
    del(run);
    del(udp);
}