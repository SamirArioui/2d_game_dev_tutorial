# ============================================================================
# Stage 17 exercises — build the drills.
#
# Two groups:
#   PURE_EXERCISES  : plain executables (frame math / controller). No SFML, so
#                     they even compile with a bare `clang++ -std=c++17 file.cpp`.
#   SFML_EXERCISES  : need a window/texture/text; they link SFML (fetched once).
#
# For each drill we build BOTH the starter (<name>) and the reference
# (<name>_solution) so you can diff your work against the answer.
#
# Usage (from this exercises/ folder):
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ./build/01_frame_at_solution           # a pure drill: prints "...passed"
#   ./build/03_texture_lifetime_solution   # an SFML drill: needs a display
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage17_exercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

# Pure drills: starter + solution, no libraries.
set(PURE_EXERCISES 01_frame_at 02_sheet_rects 06_animator)
foreach(name IN LISTS PURE_EXERCISES)
    add_executable(${name} ${name}.cpp)
    add_executable(${name}_solution ${name}.solution.cpp)
endforeach()

# SFML drills: starter + solution, linked against SFML graphics/window/system.
set(SFML_EXERCISES 03_texture_lifetime 04_procedural_texture 05_text_label)
foreach(name IN LISTS SFML_EXERCISES)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE sfml-graphics sfml-window sfml-system)
    add_executable(${name}_solution ${name}.solution.cpp)
    target_link_libraries(${name}_solution PRIVATE sfml-graphics sfml-window sfml-system)
endforeach()
