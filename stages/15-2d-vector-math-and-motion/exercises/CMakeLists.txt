# ============================================================================
# Stage 15 exercises — six Catch2 drills (all pure maths, no window).
#
# Every drill is a self-contained test file, so this project only needs Catch2
# (no SFML — the vector maths never touches a window). The *.solution targets
# are registered with CTest and pass; the starter *.cpp files start RED.
#
# Usage (from this exercises/ folder):
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ctest --test-dir build --output-on-failure     # runs the solutions
#
#   cmake --build build --target 02_normalize && ./build/02_normalize   # your turn
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage15_exercises LANGUAGES CXX)

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
    01_dot_length
    02_normalize
    03_lerp
    04_semi_implicit_euler
    05_aim_velocity
    06_easing_tween
)

foreach(name IN LISTS DRILLS)
    add_executable(${name}_solution ${name}.solution.cpp)
    target_link_libraries(${name}_solution PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${name}_solution)

    add_executable(${name} ${name}.cpp)  # starter — starts RED, run by hand
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain)
endforeach()
