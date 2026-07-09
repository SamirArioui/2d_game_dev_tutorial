# ============================================================================
# Stage 16 exercises — six Catch2 drills (all pure collision math, no window).
#
# Detection and response are just arithmetic, so every drill is a headless test
# file needing only Catch2 (no SFML). The *.solution targets are registered with
# CTest and pass; the starter *.cpp files start RED.
#
# Usage (from this exercises/ folder):
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ctest --test-dir build --output-on-failure     # runs the solutions
#
#   cmake --build build --target 03_circle_vs_circle && ./build/03_circle_vs_circle
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage16_exercises LANGUAGES CXX)

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

set(DRILLS
    01_aabb_overlap
    02_aabb_penetration
    03_circle_vs_circle
    04_circle_vs_aabb
    05_reflect_restitution
    06_resolve_and_bounce
)

foreach(name IN LISTS DRILLS)
    add_executable(${name}_solution ${name}.solution.cpp)
    target_link_libraries(${name}_solution PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${name}_solution)

    add_executable(${name} ${name}.cpp)  # starter — starts RED, run by hand
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain)
endforeach()
