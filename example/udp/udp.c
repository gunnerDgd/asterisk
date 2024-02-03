#include <asterisk/net.h>
#include <asterisk/core.h>

#include <stdio.h>

void async_main()                   {
    udp *udp = make(udp_t) from (0);
    end *end = make(end_t) from (2, v4_from_int(0), 6500);
    
    u8_t* buf = new(u8_t[64]);
    mem_set(buf, 0x00, 64);
    if (!udp_conn(udp, end)) {
        mem_del(0, buf);
        del(end);
        del(udp);

        return;
    }

    u64_t recv_size = await(udp_recv(udp, buf, 64));
    printf("Received %s (%d Bytes)\n", buf, recv_size);

    drop(buf);
    del (udp);
    del (end);
}