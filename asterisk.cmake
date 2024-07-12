if   (NOT CRISP_THREAD)
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

if   (NOT CRISP_TLS)
    message("[asterisk] asterisk requires CRISP_TLS for thread safety.")
    message(SEND_ERROR)
endif()

if   (NOT CRISP)
    message("[asterisk] asterisk requires crisp")
    message(SEND_ERROR)
endif()

macro   (asterisk_examples)
    add_subdirectory(lib/asterisk/example)
endmacro()