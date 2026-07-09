# ============================================================================
# Stage 27 capstone — top-down explorer on libengine.
#
# Same build shape as stage 26:
#   1. fetch SFML (engine dependency) + Catch2 (tests),
#   2. add the self-contained ../engine folder (STATIC `engine` library),
#   3. build the `capstone` executable (links engine),
#   4. build `capstone_tests` — the PURE game logic (collision, pickup, inventory,
#      save/load), linking only Catch2 (+ the SFML-free tilemap.cpp), so they run
#      headless under ctest.
#
# Build & test (from this folder):
#   cmake -S . -B build            # first configure clones SFML + Catch2 (network)
#   cmake --build build
#   ctest --test-dir build --output-on-failure
#   ./build/capstone               # opens a window (needs a display)
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage27_capstone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML Catch2)

# The self-contained engine (defines target `engine`).
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../engine ${CMAKE_CURRENT_BINARY_DIR}/engine)

# --- The game executable ----------------------------------------------------
add_executable(capstone
    src/main.cpp
    src/explore_scene.cpp
    src/world.cpp
    src/inventory.cpp
    src/save_data.cpp
)
target_include_directories(capstone PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(capstone PRIVATE engine)
# Absolute path to the assets folder so the game finds level.txt from any CWD.
target_compile_definitions(capstone PRIVATE
    CAPSTONE_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")

# --- The unit tests (pure logic, no SFML) -----------------------------------
# They compile the SFML-free game sources + the engine's SFML-free tilemap.cpp,
# and link only Catch2 -> window-free and fast.
enable_testing()
add_executable(capstone_tests
    tests/test_world.cpp
    tests/test_inventory.cpp
    tests/test_save_load.cpp
    src/world.cpp
    src/inventory.cpp
    src/save_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../engine/src/tilemap.cpp
)
target_include_directories(capstone_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(capstone_tests PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(capstone_tests)
