if (UNIX AND NOT APPLE)
    add_library(asterisk-net STATIC
        epoll/end.h
        epoll/end.c

        epoll/tcp.h
        epoll/tcp.c

        epoll/tcp_acpt.h
        epoll/tcp_acpt.c

        epoll/udp.h
        epoll/udp.c

        epoll/v4.h
        epoll/v4.c

        epoll/v6.h
        epoll/v6.c

        end.h
        tcp_acpt.h
        tcp.h
        udp.h
        v4.h
        v6.h
    )

    add_library(asterisk-net-include STATIC
        ../../include/net.h
        ../../include/net/end.h
        ../../include/net/tcp_acpt.h
        ../../include/net/tcp.h
        ../../include/net/udp.h
        ../../include/net/v6.h
        ../../include/net/v4.h
    )

    set_target_properties(asterisk-net-include PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(asterisk-net crisp asterisk-io)
endif ()