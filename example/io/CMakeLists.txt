add_executable            (io-01-vma vma.c)
target_include_directories(io-01-vma PRIVATE ../../include)
target_link_libraries     (io-01-vma asterisk)