cmake_minimum_required(VERSION 4.2)
project(Chip_8)
project(Assembler)

find_package(raylib 6.0 REQUIRED)
include_directories(include/utils)


set(CMAKE_CXX_STANDARD 20)

# Emulator
add_executable(Chip_8 src/emulator/main.cpp
        src/emulator/chip8.cpp
        src/emulator/debugger.cpp
        src/emulator/graphics.cpp

        include/emulator/chip8.hpp
        include/emulator/debugger.hpp
        include/emulator/graphics.hpp
	)

target_link_libraries(Chip_8 raylib)
target_include_directories(Chip_8 PRIVATE include/emulator/)


# Assembler
add_executable(Assembler src/assembler/main.cpp
		src/assembler/assembler.cpp
		include/assembler/assembler.hpp
	)

target_include_directories(Assembler PRIVATE include/assembler/)
