# ============================================================================
# Stage 14 exercises — one CMake project that fetches SFML + Catch2 once and
# builds every drill.
#
# Two kinds of drill:
#   - WINDOW drills (01-03): SFML programs. Built as plain executables you run
#     by hand and watch. Not registered with CTest (they need a display).
#   - TEST drills (04-05): Catch2 files with no window. The *.solution targets
#     are registered with CTest, so `ctest` runs them and they pass. The starter
#     *.cpp versions start RED (their function is a stub) — run them by hand.
#
# Usage (from this exercises/ folder):
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++   # first run clones deps
#   cmake --build build
#   ctest --test-dir build --output-on-failure                 # runs the solutions
#
#   # work on one drill, then run YOUR version:
#   cmake --build build --target 04_fixed_accumulator
#   ./build/04_fixed_accumulator
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage14_exercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

enable_testing()
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

# --- window drills: build starter + solution, link SFML ---------------------
set(WINDOW_DRILLS
    01_open_window
    02_coordinate_system
    03_frame_rate_independent
)
foreach(name IN LISTS WINDOW_DRILLS)
    add_executable(${name} ${name}.cpp)
    add_executable(${name}_solution ${name}.solution.cpp)
    foreach(t ${name} ${name}_solution)
        target_link_libraries(${t} PRIVATE sfml-graphics sfml-window sfml-system)
    endforeach()
endforeach()

# --- test drills: build starter + solution, link Catch2, register solution --
set(TEST_DRILLS
    04_fixed_accumulator
    05_reflect_edges
)
foreach(name IN LISTS TEST_DRILLS)
    add_executable(${name}_solution ${name}.solution.cpp)
    target_link_libraries(${name}_solution PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${name}_solution)

    add_executable(${name} ${name}.cpp)  # the starter — run by hand, starts RED
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain)
endforeach()
