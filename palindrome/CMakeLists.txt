cmake_minimum_required(VERSION 3.20)
project(template)

add_subdirectory(app)
add_subdirectory(mod1)
add_subdirectory(mod2)
add_subdirectory(mod3)

# Include header files
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Add executable
# add_executable(template src/main.c)

