cmake_minimum_required(VERSION 3.14)
project(asterisk-all)

if   (NOT PRESET_FEATURE_COLLECTIONS)
    message("[asterisk] asterisk requires \"collections\" feature of crisp.")
    message(SEND_ERROR)
endif()

if   (NOT PRESET_FEATURE_THREAD)
    message("[asterisk] asterisk requires \"thread\" feature of crisp.")
    message(SEND_ERROR)
endif()

if   (NOT DEFINED PRESET_ARCH)
    message ("[asterisk] Architecture (PRESET_ARCH_XXX) Not Defined")
    message (SEND_ERROR)
endif()

if   (NOT DEFINED PRESET_OS)
    message ("[asterisk] Base OS (PRESET_LINUX / PRESET_WIN32 / etc) Not Defined")
    message (SEND_ERROR)
endif()

if   (NOT DEFINED PRESET_COMPILER)
    message ("[asterisk] Compiler (PRESET_COMPILER_XXX) Not Defined")
    message (SEND_ERROR)
endif()

add_subdirectory(src)
if (PRESET_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()