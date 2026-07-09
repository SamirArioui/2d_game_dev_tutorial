# ============================================================================
# Stage 26 exercises — SFML-free engine-API drills.
#
# Every drill here exercises a PURE part of the engine (gmath, ResourceManager,
# SceneStack, Tilemap, the particle pool), so it needs only Catch2 — no SFML, no
# window. That is a feature of the engine's design: the testable core is cleanly
# separated from the SFML-facing rendering code.
#
# Two targets per drill:
#   - <name>_solution : the finished reference, registered with CTest (all pass).
#   - <name>          : the STARTER you edit; built as a plain executable to run
#                       by hand. Starts RED (the TODO checks are wrong on purpose).
#
# Usage (from this exercises/ folder):
#   cmake -S . -B build                        # first run clones Catch2 (network)
#   cmake --build build
#   ctest --test-dir build --output-on-failure # runs the SOLUTIONS -> all green
#
#   cmake --build build --target 01_vec2_math  # then run YOUR version:
#   ./build/01_vec2_math
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage26_exercises LANGUAGES CXX)

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

# The pure engine headers live next door; add them to the include path.
set(ENGINE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include)

set(EXERCISES
    01_vec2_math
    02_aabb_overlap
    03_resource_cache
    04_scene_stack
    05_tilemap_parse
    06_particle_pool
)

# Tilemap's parse/query methods are compiled (not header-only), but they are
# SFML-free, so the tilemap drill just compiles that one .cpp alongside it — no
# need to link the whole SFML-backed engine library.
set(TILEMAP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../engine/src/tilemap.cpp)

foreach(name IN LISTS EXERCISES)
    set(extra_src "")
    if(name STREQUAL "05_tilemap_parse")
        set(extra_src ${TILEMAP_SRC})
    endif()

    add_executable(${name}_solution ${name}.solution.cpp ${extra_src})
    target_include_directories(${name}_solution PRIVATE ${ENGINE_INCLUDE})
    target_link_libraries(${name}_solution PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${name}_solution)

    add_executable(${name} ${name}.cpp ${extra_src})
    target_include_directories(${name} PRIVATE ${ENGINE_INCLUDE})
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain)
endforeach()
