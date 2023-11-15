#include "tcp.h"
#include "tcp_acpt.h"
#include "box.h"

#include "io_sched.h"

#include <stdio.h>

io_sched_main(par_task, par) {
    tcp* tcp = make(tcp_t) from (1, par);
    if (!tcp) return 0;

    box* buf = make(box_t) from(1, 64);
    ptr  ptr = box_ptr(buf, 0);
    ptr_set_as(ptr, 0x00, ptr_size(ptr));

    task* conn = tcp_conn(tcp, "127.0.0.1", 6500);
    if  (!conn)
        return 1;

    await(conn);
    printf("Connected\n");

    task* recv = tcp_recv(tcp, ptr, 64);
    printf("Received %s (%d Bytes).\n", ptr_as(ptr, const char*), await(recv));

    return 0;
}