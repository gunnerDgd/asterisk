if    (UNIX AND NOT APPLE)
    add_library(asterisk-fs STATIC
        epoll/file.h
        epoll/file.c

        epoll/out.h
        epoll/out.c

        epoll/in.h
        epoll/in.c

        file.h
        out.h
        in.h

        ../fs.h
        ../fs.c
    )

    add_library(asterisk-fs-include STATIC
        ../../include/fs.h
        ../../include/fs/file.h
        ../../include/fs/out.h
        ../../include/fs/in.h
    )

    set_target_properties(asterisk-fs-include PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(asterisk-fs crisp asterisk-io)
endif ()

if    (WIN32)
    add_library(asterisk-fs STATIC
        iocp/file.h
        iocp/file.c

        iocp/out.h
        iocp/out.c

        iocp/in.h
        iocp/in.c

        file.h
        ../fs.h
        ../fs.c
    )

    add_library(asterisk-fs-include STATIC
        ../../include/fs.h
        ../../include/fs/file.h
        ../../include/fs/out.h
        ../../include/fs/in.h
    )

    set_target_properties(asterisk-fs-include PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(asterisk-fs crisp asterisk-io)
endif ()