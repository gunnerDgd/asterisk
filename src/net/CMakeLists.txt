if (UNIX AND NOT APPLE)
    add_library(asterisk-net STATIC
        epoll/net.h
        epoll/net.c

        epoll/end.h
        epoll/end.c

        epoll/tcp.h
        epoll/tcp.c

        epoll/acp.h
        epoll/acp.c

        epoll/acp/tcp.h
        epoll/acp/tcp.c

        epoll/udp.h
        epoll/udp.c

        epoll/v4.h
        epoll/v4.c

        epoll/v6.h
        epoll/v6.c

        end.h
        acp.h
        tcp.h
        udp.h
        v4.h
        v6.h

        ../net.h
        ../net.c
        net.h
    )

    add_library(asterisk-net-include STATIC
        ../../include/net.h
        ../../include/net/end.h
        ../../include/net/acp.h
        ../../include/net/tcp.h
        ../../include/net/udp.h
        ../../include/net/v6.h
        ../../include/net/v4.h
    )

    set_target_properties(asterisk-net-include PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(asterisk-net crisp asterisk-io)
endif ()

if (WIN32)
    add_library(asterisk-net STATIC
        iocp/net.h
        iocp/net.c

        iocp/end.h
        iocp/end.c

        iocp/tcp.h
        iocp/tcp.c

        iocp/acp.h
        iocp/acp.c

        iocp/acp/tcp.h
        iocp/acp/tcp.c

        iocp/udp.h
        iocp/udp.c

        iocp/v4.h
        iocp/v4.c

        iocp/v6.h
        iocp/v6.c

        end.h
        acp.h
        tcp.h
        udp.h
        v4.h
        v6.h

        ../net.h
        ../net.c
        net.h
    )

    add_library(asterisk-net-include STATIC
        ../../include/net.h
        ../../include/net/end.h
        ../../include/net/acp.h
        ../../include/net/tcp.h
        ../../include/net/udp.h
        ../../include/net/v6.h
        ../../include/net/v4.h
    )

    set_target_properties(asterisk-net-include PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(asterisk-net crisp asterisk-io ws2_32 Mswsock)
endif ()