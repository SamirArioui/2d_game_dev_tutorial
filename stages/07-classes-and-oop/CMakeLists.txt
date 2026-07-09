# Stage 07 — Classes & OOP
#
#   cmake -S . -B build && cmake --build build
#   ./build/battle
cmake_minimum_required(VERSION 3.16)
project(stage07_classes_and_oop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# Mini-project: several translation units (character / enemy / battle_log / main) compiled and
# linked into one executable — the multi-file pattern from stage 04, now driven by CMake.
add_executable(battle
    project/src/main.cpp
    project/src/character.cpp
    project/src/enemy.cpp
    project/src/battle_log.cpp)
target_include_directories(battle PRIVATE project/src)

# Exercise solutions: one runnable target each.
file(GLOB exercise_solutions CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/exercises/*.solution.cpp")
foreach(sol ${exercise_solutions})
    get_filename_component(fname "${sol}" NAME)
    string(REPLACE ".solution.cpp" "" stem "${fname}")
    add_executable("ex_${stem}" "${sol}")
endforeach()
