#include <asterisk/net.h>
#include <asterisk/core.h>

#include <stdio.h>
#include <stdlib.h>

void async_main()                   {
    v4  *addr = make(v4_t)  from (0);
    udp *udp  = make(udp_t) from (0);
    v4_from_cstr(addr, "127.0.0.1");
    v4_port     (addr, 6500);
    u64_t send_size = await(udp_send_to(udp, "Hello World\n", 12, addr));
    printf("Sent %d Bytes\n", send_size);
    del(udp);
}