# ============================================================================
# Stage 13 exercises — one CMake project that fetches Catch2 once and builds
# every exercise.
#
# Two kinds of target per exercise:
#   - <name>_solution : the completed reference. All solutions are registered
#                       with CTest, so `ctest` runs them and they all pass.
#   - <name>          : the STARTER you edit. Built as a plain executable you
#                       run by hand. NOT registered with CTest, because two of
#                       them (05_tdd_percent, 06_planted_bug) start RED on
#                       purpose — that's the point of those exercises.
#
# Usage (from this exercises/ folder):
#   cmake -S . -B build                       # first run clones Catch2 (network)
#   cmake --build build
#   ctest --test-dir build --output-on-failure   # runs the SOLUTIONS -> all green
#
#   # work on one exercise, then run YOUR version directly:
#   cmake --build build --target 05_tdd_percent
#   ./build/05_tdd_percent
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage13_exercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
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

set(EXERCISES
    01_first_test
    02_sections
    03_fixture
    04_generate
    05_tdd_percent
    06_planted_bug
)

foreach(name IN LISTS EXERCISES)
    # The reference solution — discovered by CTest, expected to pass.
    add_executable(${name}_solution ${name}.solution.cpp)
    target_link_libraries(${name}_solution PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${name}_solution)

    # The starter you edit — build and run it yourself to check your work.
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain)
endforeach()
