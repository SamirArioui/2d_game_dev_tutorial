# ============================================================================
# Stage 24 exercises — self-contained C++ drills (no SFML, no network).
#
# Compile one by hand:
#   clang++ -std=c++17 -Wall -Wextra 04_event_bus.cpp -o ex && ./ex
#
# Or build/run them all:
#   cmake -S . -B build && cmake --build build
#   ctest --test-dir build --output-on-failure   # runs the SOLUTIONS -> all green
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage24_exercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra)

enable_testing()

set(EXERCISES
    01_game_state
    02_state_machine
    03_input_mapping
    04_event_bus
    05_scene_stack
    06_pause_overlay
)

foreach(name IN LISTS EXERCISES)
    add_executable(${name} ${name}.cpp)

    add_executable(${name}_solution ${name}.solution.cpp)
    add_test(NAME ${name}_solution COMMAND ${name}_solution)
endforeach()
