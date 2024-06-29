if    (UNIX AND NOT APPLE)
    add_library (asterisk-io STATIC
        epoll/io_poll.h
        epoll/io_poll.c

        epoll/io_res.h
        epoll/io_res.c

        epoll/io_run.h
        epoll/io_run.c

        epoll/vma.h
        epoll/vma.c

            io_run.h
        io_poll.h
        io_res.h
        vma.h

        ../io.h
        ../io.c
    )

    add_library (asterisk-io-include STATIC
        ../../include/io.h
        ../../include/io/io_run.h
        ../../include/io/vma.h
    )

    set_target_properties(asterisk-io-include PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(asterisk-io crisp asterisk-fs)
endif ()

if   (WIN32)
    add_library(asterisk-io STATIC
            iocp/io_run.h
            iocp/io_run.c

        iocp/io_res.h
        iocp/io_res.c

        iocp/vma.h
        iocp/vma.c

            io_run.h
        io_poll.h
        io_res.h

        ../io.h
        ../io.c
    )

    add_library (asterisk-io-include STATIC
        ../../include/io.h
            ../../include/io/io_run.h
        ../../include/io/vma.h
    )

    set_target_properties(asterisk-io-include PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(asterisk-io crisp)
endif()