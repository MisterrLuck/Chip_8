cmake_minimum_required(VERSION 4.2)
project(Chip_8)

# Example if needed gtaphics library
# find_package(raylib 6.0 REQUIRED)

include_directories(include/)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chip_8 src/main.cpp
        src/chip8.cpp
        include/chip8.hpp)

#target_link_libraries(Chip_8
#        glfw
#        OpenGL::GL)
