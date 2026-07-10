# ============================================================================
# Stage 18 mini-project — Pong.  (REFERENCE SOLUTION)
# ============================================================================
# This is the complete reference: the pong rules (src/pong.cpp) and the AABB
# overlap test (src/aabb.cpp) are fully implemented, so `ctest` here is GREEN.
# The STARTER you fill in is one level up (../src/pong.cpp, ../src/aabb.cpp) —
# try it yourself first, then compare.
#
# Self-contained and NOT wired into any parent/course CMake; build it on its own:
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ctest --test-dir build --output-on-failure     # -> all green
# ============================================================================
#
# Same three-target shape as stage 17, now with AUDIO:
#   - pong_core  : SFML-FREE game logic (motion, collision, scoring, serve).
#   - pong_tests : Catch2 runner linking pong_core only -> runs under `ctest`,
#                  headless. This is where the game's rules are verified.
#   - pong       : the SFML app. Adds sfml-audio for the paddle-hit beep.
#
# Build everything (first configure clones SFML 2.6.1 + Catch2 -> needs network):
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ctest --test-dir build --output-on-failure     # green
#   ./build/pong                                     # needs a display + audio
#
# Fast logic-only loop (does NOT compile SFML):
#   cmake --build build --target pong_tests && ctest --test-dir build
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage18_pong LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# --- SFML 2.6.1, WITH audio this time ---------------------------------------
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)  # needed for sf::Sound / sf::SoundBuffer
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

# --- Catch2 -----------------------------------------------------------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

# --- the SFML-FREE game logic -----------------------------------------------
add_library(pong_core
    src/pong.cpp
    src/aabb.cpp
)
target_include_directories(pong_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- tests: pong_core + Catch2, NO SFML -------------------------------------
enable_testing()
add_executable(pong_tests
    tests/test_pong.cpp
    tests/test_collision.cpp
)
target_link_libraries(pong_tests PRIVATE pong_core Catch2::Catch2WithMain)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(pong_tests)

# --- the game -----------------------------------------------------------------
add_executable(pong src/main.cpp)
target_link_libraries(pong PRIVATE pong_core sfml-graphics sfml-window sfml-system sfml-audio)
