# Stage 11 — Smart pointers, move & the rule of 0/3/5.
#
# Self-contained and independently buildable:
#     cmake -S . -B build && cmake --build build
#     ./build/stage11_entities
#
# It is also picked up automatically by the course-root CMakeLists.txt.
cmake_minimum_required(VERSION 3.16)
project(stage11_smart_pointers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_compile_options(-Wall -Wextra)

# --- Mini-project: entities + shared resource + rule-of-5 -------------------
add_executable(stage11_entities
    project/src/main.cpp
    project/src/scoped_resource.cpp)

# --- Exercise solutions: one runnable target per *.solution.cpp -------------
file(GLOB stage11_solutions CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/exercises/*.solution.cpp")
foreach(sol ${stage11_solutions})
    get_filename_component(fname "${sol}" NAME)
    string(REPLACE ".solution.cpp" "" ename "${fname}")
    add_executable(s11_${ename} "${sol}")
endforeach()
