if    (UNIX AND NOT APPLE)
    add_library(asterisk-fs STATIC
        epoll/file.h
        epoll/file.c
    )

    add_library(asterisk-fs-include STATIC
        ../../include/fs.h
        ../../include/fs/file.h
    )

    set_target_properties(asterisk-fs-include PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(asterisk-fs crisp asterisk-io)
endif ()