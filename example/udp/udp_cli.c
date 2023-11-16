#include "udp.h"
#include "io_sched.h"

#include "box.h"

#include <stdio.h>
#include <Windows.h>

io_sched_main(par_task, par) {
    v4  *udp_addr = make(v4_t)  from (2, "127.0.0.1", 6500);
    udp *udp      = make(udp_t) from (1, par);

    box *buf      = make(box_t) from(1, 64);
    ptr  buf_ptr  = box_ptr(buf, 0);
    ptr_set_as(buf_ptr, 0x00, ptr_size(buf_ptr));
    ptr_write (buf_ptr, "Hello World\n", 12);

    while(true_t) {
        task* send      = udp_send_to(udp, buf_ptr, ptr_size(buf_ptr), udp_addr);
        u64_t send_size = await(send);

        printf("Sent %d Bytes\n", send_size);
        del   (send);
        Sleep (3000);
    }
}