#include <io.h>
#include <fs.h>
#include <core.h>

#include <stdio.h>

void run_async() {
    io_sched *io_sched_1 = make (io_sched) from (0)            ;
    file     *file_1     = make (file)     from (1, io_sched_1);
    if (!file_create_cstr(file_1, "./test.txt")) {
        if (!file_open_cstr  (file_1, "./test.txt")) {
            printf ("Failed to Create File\n");
            return;
        }
    }

    mem  *fmap = make (mem) from (2, file_map, file_1);
    void *ptr  = mem_acq(fmap, 0, 1 MB);
    return;
}