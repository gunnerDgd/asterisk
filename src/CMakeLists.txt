project         (asterisk)
add_subdirectory(io)
add_subdirectory(fs)
add_subdirectory(net)

add_library              (asterisk STATIC  main.c)
if (UNIX AND NOT APPLE)
    target_link_libraries(crisp asterisk)
endif()

target_link_libraries(asterisk
    asterisk-io
    asterisk-fs
    asterisk-net
    crisp
)