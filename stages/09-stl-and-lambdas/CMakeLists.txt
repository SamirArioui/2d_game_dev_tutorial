# Stage 09 — STL, lambdas & std::function.
#
# Self-contained and independently buildable:
#     cmake -S . -B build && cmake --build build
#     ./build/stage09_inventory
#
# It is also picked up automatically by the course-root CMakeLists.txt.
cmake_minimum_required(VERSION 3.16)
project(stage09_stl_and_lambdas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# The -Wall -Wextra we passed by hand in stages 00-04, now set once for the stage.
add_compile_options(-Wall -Wextra)

# --- Mini-project: the inventory --------------------------------------------
add_executable(stage09_inventory
    project/src/main.cpp
    project/src/inventory.cpp)

# --- Exercise solutions: one runnable target per *.solution.cpp -------------
# Each solution is a self-contained program with its own main(); building them
# all is how we verify every exercise still compiles.
file(GLOB stage09_solutions CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/exercises/*.solution.cpp")
foreach(sol ${stage09_solutions})
    get_filename_component(fname "${sol}" NAME)
    string(REPLACE ".solution.cpp" "" ename "${fname}")
    add_executable(s09_${ename} "${sol}")
endforeach()
