add_library(mod2 OBJECT
    src/mod2.c
)

target_include_directories(mod2 PUBLIC
        inc
)

