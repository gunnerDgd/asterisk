project    (asterisk)
add_library(asterisk STATIC
    env.h
    env.c

    file.h
    file.c

    io_sched.h
    io_sched.c
    
    tcp_acpt.h
    tcp_acpt.c

    tcp.h
    tcp.c

    udp.h
    udp.c
)

if(PRESET_WIN32)
    add_library(asterisk-env STATIC
        details/win32/file.h
        details/win32/file.c

        details/win32/io_sched.h
        details/win32/io_sched.c

        details/win32/io_res.h
        details/win32/io_res.c

        details/win32/tcp_acpt.h
        details/win32/tcp_acpt.c

        details/win32/tcp.h
        details/win32/tcp.c

        details/win32/udp.h
        details/win32/udp.c

        details/win32/env.h
        details/win32/env.c

        details/env.h
        details/file.h
        details/io_sched.h
        details/tcp.h
        details/tcp_acpt.h
        details/udp.h
    )
endif ()