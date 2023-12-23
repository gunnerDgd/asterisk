project    (asterisk)
add_library(asterisk STATIC
    lib.h
    curr.h
    thd.h
    task.h
    sched.h
    file.h
    io_sched.h
    io_task.h
    tcp_acpt.h
    tcp.h
    udp.h
    v4.h

    async.h
    async.c
)

add_library(asterisk-include STATIC
    ../include/run.h

    ../include/lib.h
    ../include/thd.h
    ../include/task.h
    ../include/sched.h
    ../include/file.h
    ../include/io_sched.h
    ../include/io_task.h

    ../include/tcp_acpt.h
    ../include/tcp.h
    ../include/udp.h
    ../include/v4.h

    ../include/async.h
)

set_target_properties(asterisk-include PROPERTIES LINKER_LANGUAGE C)

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
    )
endif ()