add_executable            (file-01-basic basic.c)
target_include_directories(file-01-basic PRIVATE ../../include)
target_link_libraries     (file-01-basic asterisk)

add_executable            (file-02-map file_map.c)
target_include_directories(file-02-map PRIVATE ../../include)
target_link_libraries     (file-02-map asterisk)