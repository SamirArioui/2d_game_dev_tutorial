# ============================================================================
# Stage 14 mini-project — bouncing ball (SFML window) + unit tests (Catch2).
#
# This is the first stage that pulls SFML. Like fmt (stage 12) and Catch2
# (stage 13), SFML is fetched and built from source by FetchContent, so the
# only prerequisite is a compiler + CMake + a network connection on first run.
#
# Build & run:
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ./build/ball_demo                              # the window app
#   ctest --test-dir build --output-on-failure     # the pure-logic tests
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage14_ball LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# --- SFML 2.6.1 (window + graphics) -----------------------------------------
# Build SFML as static libs so the demo is a single self-contained executable
# with no dylib-path juggling at run time.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

# --- Catch2 v3 (unit tests for the pure logic) ------------------------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

# --- the window app ---------------------------------------------------------
# It includes sim.hpp (pure) + SFML (rendering). Link the three SFML modules we
# use; each carries its own dependencies transitively.
add_executable(ball_demo src/main.cpp)
target_include_directories(ball_demo PRIVATE include)
target_link_libraries(ball_demo PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_options(ball_demo PRIVATE -Wall -Wextra)

# --- the tests (no SFML — pure logic only) ----------------------------------
enable_testing()
add_executable(tests tests/test_sim.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tests)
