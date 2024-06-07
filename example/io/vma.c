#include <core.h>
#include <io.h>
#include <fs.h>

#include <stdio.h>

use        (
    dep (io)
    dep (fs)
)

run_async()                          {
    file *back = make (file) from (0);
    if (!file_create_cstr(back, "backup.txt"))
    if (!file_open_cstr  (back, "backup.txt"))
        return 0;

    file_resize (back, 4 KB);
    vma* va = make (vma) from (2, back, 4 KB);
    printf ("VMA : %08x\n", vma_ptr(va));

    mem_copy(vma_ptr(va), "Hello World", 11);
    vma_sync(va);

    del (back);
    del (va);
}