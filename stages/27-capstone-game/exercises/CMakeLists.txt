# ============================================================================
# Stage 27 exercises — SFML-free drills on the capstone's game logic.
#
# Every drill uses only the PURE game logic (inventory, save/load, tile collision,
# pickup) + the SFML-free tilemap parser, so it needs just Catch2 — no window.
#
# Two targets per drill:
#   - <name>_solution : the finished reference, registered with CTest (all pass).
#   - <name>          : the STARTER you edit; run it by hand. Starts RED on purpose.
#
# Usage (from this exercises/ folder):
#   cmake -S . -B build                        # first run clones Catch2 (network)
#   cmake --build build
#   ctest --test-dir build --output-on-failure # runs the SOLUTIONS -> all green
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage27_exercises LANGUAGES CXX)

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

# A small static lib of the SFML-free game logic, shared by every drill. It pulls
# in the engine's tilemap.cpp (also SFML-free) that world.cpp depends on.
add_library(capstone_logic STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../project/solution/src/inventory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../project/solution/src/save_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../project/solution/src/world.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../engine/src/tilemap.cpp
)
target_include_directories(capstone_logic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../project/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include
)

set(EXERCISES
    01_inventory_totals
    02_tile_collision
    03_slide_along_wall
    04_pickup
    05_save_roundtrip
)

foreach(name IN LISTS EXERCISES)
    add_executable(${name}_solution ${name}.solution.cpp)
    target_link_libraries(${name}_solution PRIVATE capstone_logic Catch2::Catch2WithMain)
    catch_discover_tests(${name}_solution)

    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE capstone_logic Catch2::Catch2WithMain)
endforeach()
