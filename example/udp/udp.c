#include "udp.h"
#include "io_sched.h"

#include "box.h"

#include <stdio.h>
#include <Windows.h>

io_sched_main(par_task, par) {
    v4  *udp_addr = make(v4_t)  from (2, "0.0.0.0", 6500);
    udp *udp      = make(udp_t) from (1, par)            ;

    box *buf      = make(box_t) from(1, 64);
    ptr  buf_ptr  = box_ptr(buf, 0);
    
    if (!udp_conn(udp, udp_addr)) {
        del(udp_addr);
        del(udp)     ;
        del(buf)     ;

        return 0;
    }

    while(true_t)                                   {
        ptr_set_as(buf_ptr, 0x00, ptr_size(buf_ptr));

        task* recv      = udp_recv(udp, buf_ptr, ptr_size(buf_ptr));
        u64_t recv_size = await(recv);

        printf("Received %s (%d Bytes)\n", ptr_as(buf_ptr, const char*), recv_size);
        del   (recv);
        Sleep (3000);
    }
}