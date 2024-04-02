project         (asterisk)
add_subdirectory(io)
add_subdirectory(fs)
add_subdirectory(net)

add_library          (asterisk STATIC main.c)
target_link_libraries(asterisk
    asterisk-io
    asterisk-fs
    asterisk-net
    crisp
)