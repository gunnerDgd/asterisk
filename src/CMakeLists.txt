add_subdirectory(base)
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
        env/win32/file.h
        env/win32/file.c

        env/win32/io_sched.h
        env/win32/io_sched.c

        env/win32/io_res.h
        env/win32/io_res.c

        env/win32/tcp_acpt.h
        env/win32/tcp_acpt.c

        env/win32/tcp.h
        env/win32/tcp.c

        env/win32/udp.h
        env/win32/udp.c

        env/win32/env.h
        env/win32/env.c

        env/file.h
        env/io_sched.h
        env/tcp.h
        env/tcp_acpt.h
        env/udp.h
    )
elseif()
endif ()