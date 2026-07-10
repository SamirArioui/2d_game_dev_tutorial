# ============================================================================
# Stage 16 mini-project — collision demo (SFML window) + unit tests (Catch2).  (REFERENCE SOLUTION)
# ============================================================================
# This is the complete, self-contained reference build: collision.hpp is fully
# implemented, so `ctest` here is GREEN. The STARTER you fill in is two levels up
# in ../../ (../../include/collision.hpp holds the stubbed predicates).
#
# As always the window app links SFML while the tests link only Catch2, because
# collision.hpp / vec2.hpp are pure and headless. That separation is what lets us
# test every predicate and the bounce response without a display.
#
# Build & run (from anywhere — this folder is a standalone project):
#   cmake -S project/solution -B project/solution/build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build project/solution/build
#   ./project/solution/build/collision_demo                     # the window app
#   ctest --test-dir project/solution/build --output-on-failure # the pure-logic tests (GREEN)
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage16_collision LANGUAGES CXX)

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

# --- the window app ---------------------------------------------------------
add_executable(collision_demo src/main.cpp)
target_include_directories(collision_demo PRIVATE include)
target_link_libraries(collision_demo PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_options(collision_demo PRIVATE -Wall -Wextra)

# --- the tests (pure collision math — no SFML) ------------------------------
enable_testing()
add_executable(tests tests/test_collision.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tests)
