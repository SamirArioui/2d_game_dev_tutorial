# Stage 08 mini-project — REFERENCE SOLUTION (standalone build)
#
# This is the complete reference for the mini-project. It is self-contained and NOT wired into
# the stage's parent CMakeLists.txt — build it on its own to compare against your own version:
#
#   cmake -S project/solution -B build && cmake --build build
#   ./build/vec2_demo
cmake_minimum_required(VERSION 3.16)
project(stage08_mini_project_solution LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# vec2.hpp is header-only, so the mini-project is a single .cpp that includes it.
add_executable(vec2_demo src/main.cpp)
target_include_directories(vec2_demo PRIVATE include)
