#include <io.h>
#include <net.h>
#include <core.h>


use         (
    dep (io)
    dep (net)
)

run_async()                                                          {
    end *end_1 = make (end) from (2, make_v4_cstr("127.0.0.1"), 6500);
    udp *udp_1 = make (udp) from (0)                                 ;

    udp_open(udp_1, v4_t);
    u64_t len = (u64_t) await(udp_send_to(udp_1, "Hello World\n", 12, end_1));
    println("Sent %d Bytes", len);
    del(udp_1);
    del(end_1);
    return 0;
}