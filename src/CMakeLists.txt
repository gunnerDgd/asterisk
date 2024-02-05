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
    
    ../include/asterisk/net/end.h
    ../include/asterisk/net/v4.h
    ../include/asterisk/net/v6.h
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

        net/win32/end.h
        net/win32/end.c

        net/win32/v4.h
        net/win32/v4.c

        net/win32/v6.h
        net/win32/v6.c

        net/win32/tcp_acpt.h
        net/win32/tcp_acpt.c

        net/win32/tcp.h
        net/win32/tcp.c

        net/win32/udp.h
        net/win32/udp.c

        net/tcp_acpt.h
        net/tcp.h
        net/udp.h

        net/end.h
        net/v4.h
        net/v6.h
        
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
elseif (UNIX AND NOT APPLE)
    add_library(asterisk STATIC
        core/linux/epoll/core.h
        core/linux/epoll/core.c

        core/linux/epoll/heap.h
        core/linux/epoll/heap.c

        core/linux/epoll/page.h
        core/linux/epoll/page.c

        core/linux/epoll/thd.h
        core/linux/epoll/thd.c

        core/linux/epoll/io_sched.h
        core/linux/epoll/io_sched.c

        core/linux/epoll/io_res.h
        core/linux/epoll/io_res.c

        core/linux/epoll/io_poll.h
        core/linux/epoll/io_poll.c

        core/linux/epoll/this.h
        core/linux/epoll/this.c

        core/this.h
        core/thd.h
        core/io_sched.h
        core/io_res.h
        core/core.h
        core/heap.h
        core/page.h
        core.h
        main.c

        fs/linux/epoll/file.h
        fs/linux/epoll/file.c
        fs/file.h
        fs.h

        net/linux/epoll/v4.h
        net/linux/epoll/v4.c

        net/linux/epoll/v6.h
        net/linux/epoll/v6.c

        net/linux/epoll/tcp_acpt.h
        net/linux/epoll/tcp_acpt.c

        net/linux/epoll/tcp.h
        net/linux/epoll/tcp.c

        net/linux/epoll/udp.h
        net/linux/epoll/udp.c

        net/linux/epoll/end.h
        net/linux/epoll/end.c

        net/tcp_acpt.h
        net/tcp.h
        net/udp.h
        net/end.h
        net/v6.h
        net/v4.h
        net/net.h
        net.h
    )

    target_link_libraries(asterisk PRIVATE
        crisp
        crisp-arch
        crisp-arch-asm
        pthread
    )
endif ()