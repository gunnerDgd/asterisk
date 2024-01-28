add_executable            (asterisk-example-file file/file.c)
target_include_directories(asterisk-example-file PRIVATE ../include)
target_link_libraries     (asterisk-example-file PRIVATE 
    asterisk
    
    crisp 
    crisp-arch
    crisp-arch-asm

    ws2_32
    Mswsock
)

add_executable            (asterisk-example-tcp tcp/tcp.c)
target_include_directories(asterisk-example-tcp PRIVATE ../include)
target_link_libraries     (asterisk-example-tcp PRIVATE 
    asterisk 
    
    crisp 
    crisp-arch
    crisp-arch-asm

    ws2_32
    Mswsock
)

add_executable            (asterisk-example-tcp-cli tcp/cli.c)
target_include_directories(asterisk-example-tcp-cli PRIVATE ../include)
target_link_libraries     (asterisk-example-tcp-cli PRIVATE 
    asterisk
    
    crisp 
    crisp-arch
    crisp-arch-asm

    ws2_32
    Mswsock
)

add_executable            (asterisk-example-udp udp/udp.c)
target_include_directories(asterisk-example-udp PRIVATE ../include)
target_link_libraries     (asterisk-example-udp PRIVATE 
    asterisk 

    crisp 
    crisp-arch
    crisp-arch-asm

    ws2_32
    Mswsock
)

add_executable            (asterisk-example-udp-cli udp/udp_cli.c)
target_include_directories(asterisk-example-udp-cli PRIVATE ../include)
target_link_libraries     (asterisk-example-udp-cli PRIVATE 
    asterisk
    
    crisp 
    crisp-arch
    crisp-arch-asm

    ws2_32
    Mswsock
)

add_executable            (asterisk-example-task task/task.c)
target_include_directories(asterisk-example-task PRIVATE ../include)
target_link_libraries     (asterisk-example-task PRIVATE 
    asterisk 
    
    crisp 
    crisp-arch
    crisp-arch-asm
    
    ws2_32
    Mswsock
)