# ============================================================================
# Stage 18 exercises.
#
#   PURE_EXERCISES : plain executables (overlap / bounce / scoring). No SFML —
#                    they compile with a bare `clang++ -std=c++17 file.cpp`.
#   SFML_EXERCISES : window drills (input, scoreboard) — link graphics/window/system.
#   AUDIO_EXERCISES: the beep drill — additionally links sfml-audio.
#
# Usage (from exercises/):
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ./build/01_aabb_overlap_solution     # pure -> prints "...passed"
#   ./build/05_beep_solution             # audio -> press SPACE to beep
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage18_exercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)  # exercise 05 needs it
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

# Pure drills.
set(PURE_EXERCISES 01_aabb_overlap 02_paddle_bounce 03_scoring_and_serve)
foreach(name IN LISTS PURE_EXERCISES)
    add_executable(${name} ${name}.cpp)
    add_executable(${name}_solution ${name}.solution.cpp)
endforeach()

# Graphics-only drills.
set(SFML_EXERCISES 04_keyboard_input 06_scoreboard)
foreach(name IN LISTS SFML_EXERCISES)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE sfml-graphics sfml-window sfml-system)
    add_executable(${name}_solution ${name}.solution.cpp)
    target_link_libraries(${name}_solution PRIVATE sfml-graphics sfml-window sfml-system)
endforeach()

# Audio drill: also links sfml-audio.
set(AUDIO_EXERCISES 05_beep)
foreach(name IN LISTS AUDIO_EXERCISES)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
    add_executable(${name}_solution ${name}.solution.cpp)
    target_link_libraries(${name}_solution PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
endforeach()
