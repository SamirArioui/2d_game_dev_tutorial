# ============================================================================
# Stage 23 exercises — plain, self-contained C++ drills (no SFML, no network).
#
# Each drill is one .cpp you can also compile by hand, e.g.:
#   clang++ -std=c++17 -Wall -Wextra 02_component_bag.cpp -o ex && ./ex
#
# Or build/run them all via CMake:
#   cmake -S . -B build && cmake --build build
#   ctest --test-dir build --output-on-failure   # runs the SOLUTIONS -> all green
#
# For each drill we build TWO targets:
#   <name>          — the starter you edit (NOT registered with CTest; it starts incomplete)
#   <name>_solution — the reference (registered with CTest; asserts pass, exits 0)
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage23_exercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra)

enable_testing()

set(EXERCISES
    01_plain_oop
    02_component_bag
    03_movement_system
    04_new_component
    05_bounce_system
)

foreach(name IN LISTS EXERCISES)
    add_executable(${name} ${name}.cpp)

    add_executable(${name}_solution ${name}.solution.cpp)
    add_test(NAME ${name}_solution COMMAND ${name}_solution)
endforeach()
