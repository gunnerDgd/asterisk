add_executable            (udp-01-udp udp.c)
target_include_directories(udp-01-udp PRIVATE ../../include)
target_link_libraries     (udp-01-udp asterisk)

add_executable            (udp-02-udp-cli udp_cli.c)
target_include_directories(udp-02-udp-cli PRIVATE ../../include)
target_link_libraries     (udp-02-udp-cli asterisk)