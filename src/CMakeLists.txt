project    (asterisk)
add_library(asterisk-include STATIC
    ../include/asterisk/thd.h
    ../include/asterisk/this.h
    ../include/asterisk/io_run.h
    ../include/asterisk/io_serv.h

    ../include/asterisk/file.h
    ../include/asterisk/tcp_acpt.h
    ../include/asterisk/tcp.h
    ../include/asterisk/udp.h
    ../include/asterisk/v4.h
)

set_target_properties(asterisk-include PROPERTIES LINKER_LANGUAGE C)
if(PRESET_WIN32)
    add_library(asterisk STATIC
        core/win32/core.h
        core/win32/core.c

        core/win32/ws2.h
        core/win32/ws2.c

        core/win32/thd.h
        core/win32/thd.c

        core/win32/io_run.h
        core/win32/io_run.c

        core/win32/io_res.h
        core/win32/io_res.c

        core/win32/io_serv.h
        core/win32/io_serv.c

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
        io_run.h
        io_res.h
        io_serv.h

        file.h
        tcp_acpt.h
        tcp.h
        udp.h
        v4.h
    )

    target_link_libraries(asterisk PRIVATE
        crisp 
        crisp-arch
        crisp-arch-asm

        ws2_32
        Mswsock)
endif ()