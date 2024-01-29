#include <asterisk/udp.h>
#include <asterisk/this.h>
#include <asterisk/v4.h>

#include <asterisk/io_serv.h>
#include <asterisk/io_run.h>

#include <stdio.h>

void async_main()                                {
    io_run  *run  = make(io_run_t)  from (0)     ;
    udp     *udp  = make(udp_t)     from (1, run);
    io_serv *serv = make(io_serv_t) from (1, run);
    v4      *addr = make(v4_t)      from (0)     ;
    
    u8_t* buf = new(u8_t[64]);
    v4_port     (addr, 6500);
    mem_set(buf, 0x00, 64);
    if (!udp_conn(udp, addr)) {
        mem_del(0, buf);
        del(addr);
        del(udp) ;

        return 0;
    }

    u64_t recv_size = await(udp_recv(udp, buf, 64));
    printf("Received %s (%d Bytes)\n", buf, recv_size);

    drop(buf) ;
    del (serv);
    del (run) ;
    del (udp) ;
}