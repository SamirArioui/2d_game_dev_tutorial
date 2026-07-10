# ============================================================================
# Stage 13 mini-project — a Catch2 test suite for the `game` library  (REFERENCE SOLUTION)
# ============================================================================
# This is the complete reference suite: every TEST_CASE has its real assertions,
# so `ctest` here is fully GREEN. The STARTER you fill in is one level up in
# ../tests/*.cpp (the code under test and this CMakeLists are shared/complete).
#
# Reuses the SAME layout as stage 12: the game logic is a library target, and
# here the "program" that links it is a TEST RUNNER instead of the app. That is
# the payoff of stage 12 — logic in a library is linkable from tests.
#
# Build & test (from this solution/ folder):
#   cmake -S . -B build          # first run clones Catch2 (needs network)
#   cmake --build build
#   ctest --test-dir build --output-on-failure
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage13_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- the code under test: the game library (identical to stage 12) ----------
add_library(game
    src/game_math.cpp
    src/inventory.cpp
    src/save_data.cpp
)
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Catch2, fetched the same way we fetched fmt last stage ------------------
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1        # Catch2 v3 is a compiled library, not a single header
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

# enable_testing() turns on CTest for this project so `ctest` has tests to run.
enable_testing()

# The test runner executable. Catch2WithMain provides main() for us, so none of
# our test files need one.
add_executable(tests
    tests/test_math.cpp
    tests/test_inventory.cpp
    tests/test_save_load.cpp
)
target_link_libraries(tests PRIVATE game Catch2::Catch2WithMain)

# catch_discover_tests scans the built executable and registers EACH TEST_CASE
# with CTest individually, so `ctest` lists them by name and can run/filter them
# one by one. It lives in Catch2's `extras/` module directory.
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tests)
