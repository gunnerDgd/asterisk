project    (asterisk)
add_library(asterisk-include STATIC
    ../include/asterisk/core/thd.h
    ../include/asterisk/core/this.h
    ../include/asterisk/core/io_sched.h
    ../include/asterisk/core/heap.h
    ../include/asterisk/core/page.h
    ../include/asterisk/core.h

    ../include/asterisk/fs/file.h
    ../include/asterisk/fs.h

    ../include/asterisk/net/tcp_acpt.h
    ../include/asterisk/net/tcp.h
    ../include/asterisk/net/udp.h
    ../include/asterisk/net/v4.h
    ../include/asterisk/net.h
)

set_target_properties(asterisk-include PROPERTIES LINKER_LANGUAGE C)
if(PRESET_WIN32)
    add_library(asterisk STATIC
        core/win32/core.h
        core/win32/core.c

        core/win32/heap.h
        core/win32/heap.c

        core/win32/page.h
        core/win32/page.c

        core/win32/thd.h
        core/win32/thd.c

        core/win32/io_sched.h
        core/win32/io_sched.c

        core/win32/io_res.h
        core/win32/io_res.c

        core/win32/this.h
        core/win32/this.c
        
        core/this.h
        core/thd.h
        core/io_sched.h
        core/io_res.h
        core/core.h
        core/heap.h
        core/page.h
        core.h
        main.c

        fs/win32/file.h
        fs/win32/file.c
        fs/file.h
        fs.h

        net/win32/net.h
        net/win32/net.c

        net/win32/v4.h
        net/win32/v4.c

        net/win32/tcp_acpt.h
        net/win32/tcp_acpt.c

        net/win32/tcp.h
        net/win32/tcp.c

        net/win32/udp.h
        net/win32/udp.c

        net/tcp_acpt.h
        net/tcp.h
        net/udp.h
        net/v4.h
        net/net.h
        net.h
    )

    target_link_libraries(asterisk PRIVATE
        crisp 
        crisp-arch
        crisp-arch-asm

        ws2_32
        Mswsock
    )
endif ()