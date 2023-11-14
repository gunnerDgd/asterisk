add_subdirectory(details/base)
add_library     (asterisk STATIC
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
        details/env/win32/file.h
        details/env/win32/file.c

        details/env/win32/io_sched.h
        details/env/win32/io_sched.c

        details/env/win32/io_res.h
        details/env/win32/io_res.c

        details/env/win32/tcp_acpt.h
        details/env/win32/tcp_acpt.c

        details/env/win32/tcp.h
        details/env/win32/tcp.c

        details/env/win32/udp.h
        details/env/win32/udp.c

        details/env/win32/env.h
        details/env/win32/env.c

        details/env/file.h
        details/env/io_sched.h
        details/env/tcp.h
        details/env/tcp_acpt.h
        details/env/udp.h
    )
endif ()