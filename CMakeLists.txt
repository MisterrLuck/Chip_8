cmake_minimum_required(VERSION 4.2)
project(Chip_8)
project(Debug_Chip_8)

find_package(raylib 6.0 REQUIRED)
include_directories(include/)

# Configurations
add_compile_definitions(SUPER_CHIP)
# add_compile_definitions(COSMIC)


set(CMAKE_CXX_STANDARD 20)

add_executable(Chip_8 src/main.cpp
        src/chip8.cpp
		src/graphics.cpp
        include/chip8.hpp
		include/graphics.hpp
	)

target_link_libraries(Chip_8 raylib)

add_executable(Debug_Chip_8 src/main.cpp
        src/chip8.cpp
		src/graphics.cpp
		src/debugger.cpp
        include/chip8.hpp
		include/graphics.hpp
		include/debugger.hpp
	)

target_link_libraries(Debug_Chip_8 raylib)

target_compile_definitions(Debug_Chip_8 PRIVATE DEBUGGER)
