cmake_minimum_required(VERSION 4.2)
project(Chip_8)
project(Debugger)
project(Assembler)

find_package(raylib 6.0 REQUIRED)

# Configurations
add_compile_definitions(SUPER_CHIP)
# add_compile_definitions(COSMIC)


set(CMAKE_CXX_STANDARD 20)

# Emulator
add_executable(Chip_8 src/emulator/main.cpp
        src/emulator/chip8.cpp
		src/emulator/graphics.cpp
        include/emulator/chip8.hpp
		include/emulator/graphics.hpp
	)

target_include_directories(Chip_8 PRIVATE include/emulator/)
target_link_libraries(Chip_8 raylib)
target_include_directories(Chip_8 PRIVATE include/emulator/)

# Debugger
add_executable(Debugger src/emulator/main.cpp
        src/emulator/chip8.cpp
		src/emulator/graphics.cpp
		src/emulator/debugger.cpp
        include/emulator/chip8.hpp
		include/emulator/graphics.hpp
		include/emulator/debugger.hpp
	)

target_link_libraries(Debugger raylib)
target_include_directories(Debugger PRIVATE include/emulator/)
target_compile_definitions(Debugger PRIVATE DEBUGGER)

# Assembler
add_executable(Assembler src/assembler/main.cpp
		src/assembler/assembler.cpp
		include/assembler/assembler.hpp
	)

target_include_directories(Assembler PRIVATE include/assembler/)
