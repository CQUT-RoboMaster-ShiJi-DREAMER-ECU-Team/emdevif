
add_library(emdevif_compile_options INTERFACE)

if (MSVC)
    target_compile_options(emdevif_compile_options INTERFACE
        /W4 /source-charset:utf-8 /execution-charset:gb2312
    )
else ()
    target_compile_options(emdevif_compile_options INTERFACE
        -Wall -Wextra -Wpedantic
    )
endif ()
