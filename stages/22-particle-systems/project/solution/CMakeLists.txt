# ============================================================================
# Stage 22 mini-project — an explosion burst + a thruster trail from one pool.  (REFERENCE SOLUTION)
# ============================================================================
# This is the COMPLETE, self-contained reference build. The STARTER you fill in
# is one level up in ../ (same CMakeLists, same targets); only src/particle_pool.cpp
# differs — here it is fully implemented, so `ctest` is GREEN.
#
# Same three-target shape as stages 20-21:
#   particle_core  : the PURE pool + emitters. No SFML.
#   particle_demo  : the SFML app (ship, trail, click-to-explode).
#   particle_tests : Catch2 tests over the pool invariants. No SFML.
#
# Build & run (from this solution/ folder):
#   cmake -S . -B build          # first run clones SFML 2.6.1 + Catch2 v3.7.1
#   cmake --build build
#   ctest --test-dir build --output-on-failure   # pool tests -> green
#   ./build/particle_demo
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage22_particles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- pure logic library: the pool + emitters, linked by app AND tests -------
add_library(particle_core src/particle_pool.cpp)
target_include_directories(particle_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)

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

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

# --- the SFML app (build+link is the bar) -----------------------------------
add_executable(particle_demo
    src/main.cpp
    src/particle_render.cpp
)
target_link_libraries(particle_demo PRIVATE particle_core sfml-graphics sfml-window sfml-system)

# --- the test runner (pure logic, no SFML) ----------------------------------
enable_testing()
add_executable(particle_tests tests/test_pool.cpp)
target_link_libraries(particle_tests PRIVATE particle_core Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(particle_tests)
