project    (asterisk)
add_library(asterisk STATIC
    run.h

    task.h
    task.c

    thd.h
    thd.c

    sched.h
    sched.c

    lib.h
    lib.c

    file.h
    file.c

    io_sched.h
    io_sched.c

    io_task.h
    io_task.c

    async.h
    async.c

    v4.h
    v4.c
    
    tcp_acpt.h
    tcp_acpt.c

    tcp.h
    tcp.c

    udp.h
    udp.c
)

if(PRESET_WIN32)
    add_library(asterisk-env STATIC
        details/win32/lib.h
        details/win32/lib.c

        details/win32/sched.h
        details/win32/sched.c

        details/win32/task.h
        details/win32/task.c

        details/win32/io_sched.h
        details/win32/io_sched.c

        details/win32/io_task.h
        details/win32/io_task.c

        details/win32/thd.h
        details/win32/thd.c

        details/win32/curr.h
        details/win32/curr.c

        details/win32/file.h
        details/win32/file.c

        details/win32/v4.h
        details/win32/v4.c

        details/win32/tcp_acpt.h
        details/win32/tcp_acpt.c

        details/win32/tcp.h
        details/win32/tcp.c

        details/win32/udp.h
        details/win32/udp.c

        

        details/lib.h
        details/sched.h
        details/task.h
        details/io_sched.h
        details/io_task.h
        details/thd.h
        details/curr.h
        details/file.h
        details/v4.h
        details/tcp.h
        details/tcp_acpt.h
        details/udp.h
    )
endif ()