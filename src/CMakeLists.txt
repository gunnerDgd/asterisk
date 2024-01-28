project    (asterisk)
add_library(asterisk-include STATIC
    ../include/thd.h
    ../include/this.h
    ../include/io_sched.h

    ../include/file.h
    ../include/tcp_acpt.h
    ../include/tcp.h
    ../include/udp.h
    ../include/v4.h
)

set_target_properties(asterisk-include PROPERTIES LINKER_LANGUAGE C)
if(PRESET_WIN32)
    add_library(asterisk STATIC
        core/win32/core.h
        core/win32/core.c

        core/win32/thd.h
        core/win32/thd.c

        core/win32/io_sched.h
        core/win32/io_sched.c

        core/win32/io_res.h
        core/win32/io_res.c

        core/win32/this.h
        core/win32/this.c

        core/win32/file.h
        core/win32/file.c

        core/win32/v4.h
        core/win32/v4.c

        core/win32/tcp_acpt.h
        core/win32/tcp_acpt.c

        core/win32/tcp.h
        core/win32/tcp.c

        core/win32/udp.h
        core/win32/udp.c

        core.h
        this.h
        thd.h
        io_sched.h
        io_res.h

        file.h
        tcp_acpt.h
        tcp.h
        udp.h
        v4.h
    )
endif ()