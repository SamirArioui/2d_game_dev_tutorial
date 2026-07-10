# ============================================================================
# Stage 15 mini-project — motion demo (SFML window) + unit tests (Catch2).  (REFERENCE SOLUTION)
# ============================================================================
# This is the complete, self-contained reference build: vec2.hpp and motion.hpp
# are fully implemented, so `ctest` here is GREEN. The STARTER you fill in is two
# levels up in ../../ (../../include/vec2.hpp + ../../include/motion.hpp).
#
# The window app links SFML; the tests do NOT (vec2.hpp / motion.hpp are pure),
# which is the whole reason we keep the vector maths out of the SFML bridge.
#
# Build & run (from anywhere — this folder is a standalone project):
#   cmake -S project/solution -B project/solution/build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build project/solution/build
#   ./project/solution/build/motion_demo                        # the window app
#   ctest --test-dir project/solution/build --output-on-failure # the pure-logic tests (GREEN)
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage15_motion LANGUAGES CXX)

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

# --- the window app (needs SFML for rendering + the bridge header) ----------
add_executable(motion_demo src/main.cpp)
target_include_directories(motion_demo PRIVATE include)
target_link_libraries(motion_demo PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_options(motion_demo PRIVATE -Wall -Wextra)

# --- the tests (pure maths — no SFML) ---------------------------------------
enable_testing()
add_executable(tests tests/test_vec2.cpp tests/test_motion.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tests)
