add_executable       (asterisk-example-file file/file.c)
target_link_libraries(asterisk-example-file PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm
    build-preset 
)

add_executable       (asterisk-example-tcp tcp/tcp.c)
target_link_libraries(asterisk-example-tcp PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm
    build-preset 
)

add_executable       (asterisk-example-tcp-cli tcp/cli.c)
target_link_libraries(asterisk-example-tcp-cli PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm
    build-preset 
)