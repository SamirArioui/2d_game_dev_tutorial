# Stage 08 — Operator overloading, templates & namespaces
#
#   cmake -S . -B build && cmake --build build
#   ./build/vec2_demo
cmake_minimum_required(VERSION 3.16)
project(stage08_operators_templates_namespaces LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# The math lib is header-only (templates live entirely in headers — see the README for why), so
# the mini-project is a single .cpp that includes vec2.hpp from project/include.
add_executable(vec2_demo project/src/main.cpp)
target_include_directories(vec2_demo PRIVATE project/include)

# Exercise solutions: one runnable target each.
file(GLOB exercise_solutions CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/exercises/*.solution.cpp")
foreach(sol ${exercise_solutions})
    get_filename_component(fname "${sol}" NAME)
    string(REPLACE ".solution.cpp" "" stem "${fname}")
    add_executable("ex_${stem}" "${sol}")
endforeach()
