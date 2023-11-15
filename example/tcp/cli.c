#include "tcp.h"
#include "tcp_acpt.h"

#include "io_sched.h"

void* async_main(task* par_task, io_sched* par) {
    tcp* tcp = make(tcp_t) from(1, par);
    if (!tcp) return 0;

    mem buf = mem_init();
}

int main() {
    
}