cmake_minimum_required(VERSION 4.2)
project(Chip_8)
project(Debug_Chip_8)

find_package(raylib 6.0 REQUIRED)

# Configurations
add_compile_definitions(SUPER_CHIP)
# add_compile_definitions(COSMIC)


set(CMAKE_CXX_STANDARD 20)

add_executable(Chip_8 src/emulator/main.cpp
        src/emulator/chip8.cpp
		src/emulator/graphics.cpp
        include/emulator/chip8.hpp
		include/emulator/graphics.hpp
	)

target_include_directories(Chip_8 PRIVATE include/emulator/)
target_link_libraries(Chip_8 raylib)

add_executable(Debug_Chip_8 src/emulator/main.cpp
        src/emulator/chip8.cpp
		src/emulator/graphics.cpp
		src/emulator/debugger.cpp
        include/emulator/chip8.hpp
		include/emulator/graphics.hpp
		include/emulator/debugger.hpp
	)


target_include_directories(Debug_Chip_8 PRIVATE include/emulator/)
target_link_libraries(Debug_Chip_8 raylib)

target_compile_definitions(Debug_Chip_8 PRIVATE DEBUGGER)
