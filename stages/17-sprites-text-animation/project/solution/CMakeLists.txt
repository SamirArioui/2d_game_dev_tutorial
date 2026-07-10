# ============================================================================
# Stage 17 mini-project — animated walk-cycle sprite + label.  (REFERENCE SOLUTION)
# ============================================================================
# This is the complete reference: the `anim` bodies in src/animation.cpp are
# fully implemented, so `ctest` here is GREEN. The STARTER you fill in is one
# level up (../src/animation.cpp) — try it yourself first, then compare.
#
# Self-contained and NOT wired into any parent/course CMake; build it on its own:
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ctest --test-dir build --output-on-failure     # -> all green
# ============================================================================
#
# Three targets from one project, which is the pattern every game stage uses:
#   - anim        : the SFML-FREE logic library (frame math + controller).
#   - anim_tests  : a Catch2 runner linking `anim` only. NO SFML, so it builds
#                   and runs on a headless machine (this is what `ctest` runs).
#   - walk_cycle  : the SFML app (window/sprite/text). Build + link is the bar;
#                   its on-screen behavior is not run in the headless CI here.
#
# Build everything (first configure clones SFML + Catch2 -> needs network):
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ctest --test-dir build --output-on-failure     # runs anim_tests -> green
#
# Build JUST the tests (fast — does not compile SFML at all):
#   cmake --build build --target anim_tests
#   ctest --test-dir build --output-on-failure
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage17_sprites LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# --- SFML 2.6.1 (the 2.6 API; NOT 3.x) --------------------------------------
# Turn off the parts we don't need so the first build is smaller/faster. We
# build static libs so the app is self-contained.
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)  # stage 17 has no sound (18 does)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

# --- Catch2 (same as stage 13) ----------------------------------------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

# ---------------------------------------------------------------------------
# The SFML-FREE logic library. Everything testable lives here.
# ---------------------------------------------------------------------------
add_library(anim src/animation.cpp)
target_include_directories(anim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ---------------------------------------------------------------------------
# Tests: link only the logic library + Catch2. No SFML dependency on purpose.
# ---------------------------------------------------------------------------
enable_testing()
add_executable(anim_tests tests/test_animation.cpp)
target_link_libraries(anim_tests PRIVATE anim Catch2::Catch2WithMain)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(anim_tests)

# ---------------------------------------------------------------------------
# The SFML application.
# ---------------------------------------------------------------------------
add_executable(walk_cycle src/main.cpp)
target_link_libraries(walk_cycle PRIVATE anim sfml-graphics sfml-window sfml-system)
