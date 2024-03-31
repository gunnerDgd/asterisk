project         (asterisk)
add_subdirectory(io)
add_subdirectory(fs)
add_subdirectory(net)

add_library          (asterisk INTERFACE)
target_link_libraries(asterisk INTERFACE
    asterisk-io
    asterisk-fs
    asterisk-net
)