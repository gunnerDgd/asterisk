#include <core.h>
#include <io.h>
#include <fs.h>

use        (
    dep (io)
    dep (fs)
)

run_async()                            {
    file *back = new_file ("./vma.txt");

    file_trunc(back, 4 KB);
    vma* va = make (vma) from (2, back, 4 KB);
    println("VMA : %08x\n", vma_ptr(va));

    mem_copy(vma_ptr(va), L"Hello World", sizeof(L"Hello World"));
    vma_sync(va);

    del (back);
    del (va);
}