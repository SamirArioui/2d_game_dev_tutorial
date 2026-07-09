# ============================================================================
# Stage 19 exercises.
#
#   PURE_EXERCISES : plain executables (speed rescale, frame profiler). No SFML —
#                    compile with a bare `clang++ -std=c++17 file.cpp`.
#   IMGUI_EXERCISES: ImGui drills; link imgui-sfml (which pulls in ImGui + SFML).
#
# The first configure clones SFML 2.6.1 + Dear ImGui + imgui-sfml (network).
#
# Usage (from exercises/):
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ./build/01_speed_scale_solution    # pure -> "...passed"
#   ./build/04_imgui_slider_solution   # imgui -> drag the slider
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage19_exercises LANGUAGES CXX)

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

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.90.9
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_SFML_IMGUI_DEMO OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v2.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ImGui-SFML)

# Pure drills.
set(PURE_EXERCISES 01_speed_scale 02_frame_profiler)
foreach(name IN LISTS PURE_EXERCISES)
    add_executable(${name} ${name}.cpp)
    add_executable(${name}_solution ${name}.solution.cpp)
endforeach()

# ImGui drills.
set(IMGUI_EXERCISES 03_imgui_hello 04_imgui_slider 05_fps_overlay)
foreach(name IN LISTS IMGUI_EXERCISES)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE ImGui-SFML::ImGui-SFML)
    add_executable(${name}_solution ${name}.solution.cpp)
    target_link_libraries(${name}_solution PRIVATE ImGui-SFML::ImGui-SFML)
endforeach()
