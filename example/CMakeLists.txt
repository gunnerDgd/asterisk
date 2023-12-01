include_directories  (../include)

add_executable       (asterisk-example-file file/file.c)
target_link_libraries(asterisk-example-file PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm

    ws2_32
    Mswsock
)

add_executable       (asterisk-example-tcp tcp/tcp.c)
target_link_libraries(asterisk-example-tcp PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm

    ws2_32
    Mswsock
)

add_executable       (asterisk-example-tcp-cli tcp/cli.c)
target_link_libraries(asterisk-example-tcp-cli PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm

    ws2_32
    Mswsock
)

add_executable       (asterisk-example-udp udp/udp.c)
target_link_libraries(asterisk-example-udp PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm

    ws2_32
    Mswsock
)

add_executable       (asterisk-example-udp-cli udp/udp_cli.c)
target_link_libraries(asterisk-example-udp-cli PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm

    ws2_32
    Mswsock
)

add_executable       (asterisk-example-task task/task.c)
target_link_libraries(asterisk-example-task PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm
    
    ws2_32
    Mswsock
)