project         (asterisk)
add_subdirectory(io)
add_subdirectory(fs)
add_subdirectory(net)

add_library          (asterisk-main STATIC  main.c)
if (UNIX AND NOT APPLE)
    target_link_libraries(crisp asterisk-main)
endif()

add_library          (asterisk INTERFACE)
target_link_libraries(asterisk INTERFACE
    asterisk-io
    asterisk-fs
    asterisk-net
    asterisk-main
    crisp
)