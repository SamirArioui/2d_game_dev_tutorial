# ============================================================================
# Stage 26 mini-project — Pong on libengine.
#
# This is the top-level build for the stage. It:
#   1. fetches SFML (the engine's dependency) and Catch2 (for the logic tests),
#   2. adds the ../engine folder, which defines the STATIC `engine` library,
#   3. builds the `pong` executable that links `engine`,
#   4. builds `pong_tests` — the PURE-logic tests, which link only Catch2 (no
#      SFML), so they compile fast and run headless under ctest.
#
# Build & test (from this folder):
#   cmake -S . -B build            # first configure clones SFML + Catch2 (network)
#   cmake --build build
#   ctest --test-dir build --output-on-failure
#   ./build/pong                   # opens a window (needs a display)
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage26_pong LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# --- SFML 2.6.1 (the engine's rendering/window/audio backend) ---------------
# Trim the parts we don't use so the first build is smaller/faster.
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)

# --- Catch2 v3.7.1 (unit tests for the pure logic) --------------------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SFML Catch2)

# --- The engine, built as a static library (defines target `engine`) --------
# The source lives in the sibling engine/ folder, so we point add_subdirectory at
# it and give it an explicit binary directory (required when the source is
# outside this project's tree).
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../engine ${CMAKE_CURRENT_BINARY_DIR}/engine)

# --- The game executable ----------------------------------------------------
add_executable(pong
    src/main.cpp
    src/pong_scene.cpp
    src/pong_logic.cpp
)
# Linking `engine` also pulls in SFML transitively (it was linked PUBLIC).
target_link_libraries(pong PRIVATE engine)

# --- The unit tests ---------------------------------------------------------
# NOTE: the tests deliberately do NOT link `engine`/SFML. They exercise only the
# pure headers (gmath) + the pure pong_logic.cpp, so they need just Catch2. That
# keeps them window-free and fast — the whole point of splitting logic from I/O.
enable_testing()
add_executable(pong_tests
    tests/test_collision.cpp
    tests/test_pong_logic.cpp
    src/pong_logic.cpp
)
target_include_directories(pong_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(pong_tests PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(pong_tests)
