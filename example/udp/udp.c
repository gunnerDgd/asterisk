#include <asterisk/net.h>
#include <asterisk/core.h>

#include <stdio.h>

void async_main()                   {
    udp *udp  = make(udp_t) from (0);
    v4  *addr = make(v4_t)  from (0);
    
    u8_t* buf = new(u8_t[64]);
    v4_port(addr, 6500)   ;
    mem_set(buf, 0x00, 64);
    if (!udp_conn(udp, addr)) {
        mem_del(0, buf);
        del(addr);
        del(udp) ;

        return;
    }

    u64_t recv_size = await(udp_recv(udp, buf, 64));
    printf("Received %s (%d Bytes)\n", buf, recv_size);

    drop(buf) ;
    del (udp) ;
}