cmake_minimum_required(VERSION 3.19)
project(game_of_life)

set(CMAKE_CXX_STANDARD 14)
find_package(SDL2 REQUIRED SDL2)
add_executable(game_of_life main.cpp imports.h gui.cpp declarations.h)
target_link_libraries(game_of_life PRIVATE SDL2)

