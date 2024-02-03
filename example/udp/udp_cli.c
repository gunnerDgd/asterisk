#include <asterisk/net.h>
#include <asterisk/core.h>

#include <stdio.h>
#include <stdlib.h>

void async_main()                                                   {
    end *end = make(end_t) from (2, v4_from_cstr("127.0.0.1"), 6500);
    udp *udp = make(udp_t) from (0);
    u64_t send_size = await(udp_send_to(udp, "Hello World\n", 12, end));
    printf("Sent %d Bytes\n", send_size);
    del(udp);
    del(end);
    return;
}