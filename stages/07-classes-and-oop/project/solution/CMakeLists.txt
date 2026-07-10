# Stage 07 mini-project — REFERENCE SOLUTION (standalone build)
#
# This is the complete reference for the mini-project. It is self-contained and NOT wired into
# the stage's parent CMakeLists.txt — build it on its own to compare against your own version:
#
#   cmake -S project/solution -B build && cmake --build build
#   ./build/battle
cmake_minimum_required(VERSION 3.16)
project(stage07_mini_project_solution LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# Several translation units (character / enemy / battle_log / main) linked into one executable.
add_executable(battle
    src/main.cpp
    src/character.cpp
    src/enemy.cpp
    src/battle_log.cpp)
target_include_directories(battle PRIVATE src)
