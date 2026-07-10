# ============================================================================
# Stage 20 mini-project — a tile level you can render and walk around.  (REFERENCE SOLUTION)
# ============================================================================
# This is the COMPLETE, self-contained reference build. The STARTER you fill in
# is one level up in ../ (same CMakeLists, same targets); only src/tilemap.cpp
# differs — here it is fully implemented, so `ctest` is GREEN.
#
# THREE targets, and the split between them is the whole point of the stage:
#   tilemap_core  : the PURE map logic (parse/tile_at/solid_at). No SFML.
#   tilemap_demo  : the SFML app that renders the map + moves a character.
#   tilemap_tests : Catch2 tests over tilemap_core. Link Catch2, NOT SFML.
#
# Because the tests link the pure core and nothing else, `ctest` runs fast and
# needs no display.
#
# Build & run everything (from this solution/ folder):
#   cmake -S . -B build           # first run clones SFML + Catch2 (needs network)
#   cmake --build build
#   ctest --test-dir build --output-on-failure   # -> parser tests, all green
#   ./build/tilemap_demo          # the window (run from this folder for assets/)
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage20_tilemaps LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- pure logic library: linked by BOTH the app and the tests ---------------
add_library(tilemap_core src/tilemap.cpp)
target_include_directories(tilemap_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)

# --- SFML 2.6.1 (rendering) -------------------------------------------------
# Turn off the modules we don't use so the first build is smaller/faster.
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

# --- Catch2 v3 (tests) ------------------------------------------------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

# --- the SFML app (build+link is the bar; window behaviour unverified) ------
add_executable(tilemap_demo
    src/main.cpp
    src/tilemap_render.cpp
)
target_link_libraries(tilemap_demo PRIVATE tilemap_core sfml-graphics sfml-window sfml-system)

# --- the test runner (pure logic, no SFML) ----------------------------------
enable_testing()
add_executable(tilemap_tests tests/test_tilemap.cpp)
target_link_libraries(tilemap_tests PRIVATE tilemap_core Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tilemap_tests)
