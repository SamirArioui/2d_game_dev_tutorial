# Stage 10 — File I/O & error handling.
#
# Self-contained and independently buildable:
#     cmake -S . -B build && cmake --build build
#     ./build/stage10_save_load
#
# It is also picked up automatically by the course-root CMakeLists.txt.
cmake_minimum_required(VERSION 3.16)
project(stage10_file_io_and_error_handling LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_compile_options(-Wall -Wextra)

# --- Mini-project: save/load ------------------------------------------------
add_executable(stage10_save_load
    project/src/main.cpp
    project/src/savegame.cpp)

# --- Exercise solutions: one runnable target per *.solution.cpp -------------
file(GLOB stage10_solutions CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/exercises/*.solution.cpp")
foreach(sol ${stage10_solutions})
    get_filename_component(fname "${sol}" NAME)
    string(REPLACE ".solution.cpp" "" ename "${fname}")
    add_executable(s10_${ename} "${sol}")
endforeach()
