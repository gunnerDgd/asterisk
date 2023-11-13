project(asterisk-all)
set    (CMAKE_C_STANDARD 11)

set(PRESET_X86_64   ON)
set(PRESET_X86     OFF)
set(PRESET_AARCH64 OFF)

set                    (PRESET_ENV_CSTD ON)
add_compile_definitions(PRESET_ENV_CSTD)

if    (PRESET_X86_64)
    add_compile_definitions(PRESET_X86_64)
elseif(PRESET_X86)
    add_compile_definitions(PRESET_X86)
elseif(PRESET_AARCH64)
    add_compile_definitions(PRESET_AARCH64)
endif ()

if    (UNIX AND NOT APPLE)
    set                    (PRESET_LINUX ON)
    add_compile_definitions(PRESET_LINUX)
elseif(WIN32)
    set                    (PRESET_WIN32 ON)
    add_compile_definitions(PRESET_WIN32)
endif ()

add_subdirectory(src)