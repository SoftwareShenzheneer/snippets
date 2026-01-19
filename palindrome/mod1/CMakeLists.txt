add_library(mod1 OBJECT
    src/mod1.c
)

target_include_directories(mod1 PUBLIC
        inc
)

