# Stage 10 mini-project — Save/load (REFERENCE SOLUTION)
#
# Self-contained: build the finished mini-project on its own to compare against
# your starter in ../src. This tree is NOT wired into the stage or course-root
# CMakeLists — it is only built when you point CMake straight at it:
#
#     cmake -S project/solution -B build-solution && cmake --build build-solution
#     cd build-solution && ./stage10_save_load     # run from build dir so savegame.txt lands there
cmake_minimum_required(VERSION 3.16)
project(stage10_save_load_solution LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_compile_options(-Wall -Wextra)

add_executable(stage10_save_load
    src/main.cpp
    src/savegame.cpp)
