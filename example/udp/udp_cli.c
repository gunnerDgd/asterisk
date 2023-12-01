#include <run.h>
#include <udp.h>
#include <async.h>

#include <stdio.h>
#include <stdlib.h>

run()                                                      {
    v4  *udp_addr = make(v4_t)  from (2, "127.0.0.1", 6500);
    udp *udp      = make(udp_t) from (0)                   ;

    u64_t send_size = await(udp_send_to(udp, "Hello World\n", 12, udp_addr));
    printf("Sent %d Bytes\n", send_size);
}