# Stage 11 mini-project — Entities + ScopedResource (REFERENCE SOLUTION)
#
# Self-contained: build the finished mini-project on its own to compare against
# your starter in ../src. This tree is NOT wired into the stage or course-root
# CMakeLists — it is only built when you point CMake straight at it:
#
#     cmake -S project/solution -B build-solution && cmake --build build-solution
#     ./build-solution/stage11_entities
cmake_minimum_required(VERSION 3.16)
project(stage11_entities_solution LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_compile_options(-Wall -Wextra)

add_executable(stage11_entities
    src/main.cpp
    src/scoped_resource.cpp)
