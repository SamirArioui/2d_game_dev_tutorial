# Root build for the whole course.
#
# Not every stage uses CMake: stages 00-04 are compiled by hand with clang++/g++
# (that's the point — you learn what the compiler and linker actually do). CMake is
# introduced in stage 05, so only stages that contain their own CMakeLists.txt are
# wired up here. This file automatically picks up each stage as it gets authored, so
# you can also just build one stage from its own folder:
#
#     cmake -S stages/05-cmake-and-debugging-basics -B build && cmake --build build
#
cmake_minimum_required(VERSION 3.16)
project(cpp_2d_game_dev_tutorial LANGUAGES CXX)

# Course-wide C++ standard. Individual stages inherit this.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to a Debug build (symbols on, assertions on) unless told otherwise, so the
# debugging stages "just work". Override with -DCMAKE_BUILD_TYPE=Release.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Export compile_commands.json so clang-tidy and editors can find the flags.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add every stage that has a CMakeLists.txt (i.e. has been authored and uses CMake).
file(GLOB stage_dirs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/stages"
     "${CMAKE_CURRENT_SOURCE_DIR}/stages/*")
list(SORT stage_dirs)
foreach(stage ${stage_dirs})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/stages/${stage}/CMakeLists.txt")
        message(STATUS "Adding stage: ${stage}")
        add_subdirectory("stages/${stage}")
    endif()
endforeach()
