cmake_minimum_required(VERSION 4.2)
project(Chip_8)

find_package(raylib 6.0 REQUIRED)
include_directories(include/)

# Configure the version of chip8
add_compile_definitions(SHIFT)
add_compile_definitions(JUMP_OFFSET)


set(CMAKE_CXX_STANDARD 20)

add_executable(Chip_8 src/main.cpp
        src/chip8.cpp
		src/graphics.cpp
        include/chip8.hpp
		include/graphics.hpp)

target_link_libraries(Chip_8 raylib)
