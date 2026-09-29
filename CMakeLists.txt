cmake_minimum_required(VERSION 4.2)
project(Chip_8)

# find_package(glfw3 REQUIRED)
# find_package(OpenGL REQUIRED)
# find_package(SDL2 REQUIRED)
find_package(raylib 6.0 REQUIRED)
include_directories(include/)


set(CMAKE_CXX_STANDARD 20)

add_executable(Chip_8 src/main.cpp
        src/chip8.cpp
		src/graphics.cpp
        include/chip8.hpp
		include/graphics.hpp)

target_link_libraries(Chip_8 raylib)
# target_link_libraries(Chip_8 ${SDL2_LIBRARIES})
# target_link_libraries(Chip_8 glfw)
# target_link_libraries(Chip_8 OpenGL::GL)
