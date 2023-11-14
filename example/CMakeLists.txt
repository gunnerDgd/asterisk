add_executable       (asterisk-example-file file/file.c)
target_link_libraries(asterisk-example-file PRIVATE 
    asterisk 
    asterisk-env 
    
    build 
    build-arch
    build-arch-asm
    build-preset 
)