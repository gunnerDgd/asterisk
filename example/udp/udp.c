#include <run.h>
#include <udp.h>
#include <async.h>

#include <stdio.h>

run() {
    v4   *udp_addr = make(v4_t)  from (2, "0.0.0.0", 6500);
    udp  *udp      = make(udp_t) from (0)                 ;
    u8_t *buf      = mem_new(0, 64)                       ;
    
    mem_set(buf, 0x00, 64);
    if (!udp_conn(udp, udp_addr)) {
        mem_del(0, buf);
        del(udp_addr)  ;
        del(udp)       ;

        return 0;
    }

    while(true_t)                                      {
        u64_t recv_size = await(udp_recv(udp, buf, 64));
        printf("Received %s (%d Bytes)\n", buf, recv_size);
    }

    mem_del(0, buf);
}