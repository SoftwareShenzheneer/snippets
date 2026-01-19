add_executable(app
    src/main.c
)

target_link_libraries(app PRIVATE
        mod1
        mod2
        mod3
)

target_include_directories(app PUBLIC
        inc
)

set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

