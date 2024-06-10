#include <io.h>
#include <net.h>
#include <core.h>

use        (
    dep (io)
    dep (net)
)

run_async()                                                {
    end  *end_1 = make (end) from (2, make_v4_int(0), 6500);
    udp  *udp_1 = make (udp) from (0)                      ;
    u8_t *buf   = new  (u8_t[64]);
    mem_set(buf, 0x00, 64);
    if (!udp_conn(udp_1, end_1)) {
        drop(buf)  ;
        del (end_1);
        del (udp_1);

        return 0;
    }

    u64_t len = (u64_t) await(udp_recv(udp_1, buf, 64));
    println("Received %s (%d Bytes)\n", buf, len);

    drop(buf);
    del (udp_1);
    del (end_1);
}