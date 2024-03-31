add_executable            (tcp-01-tcp tcp.c)
target_include_directories(tcp-01-tcp PRIVATE ../../include)
target_link_libraries     (tcp-01-tcp asterisk)

add_executable            (tcp-02-tcp-cli cli.c)
target_include_directories(tcp-02-tcp-cli PRIVATE ../../include)
target_link_libraries     (tcp-02-tcp-cli asterisk)