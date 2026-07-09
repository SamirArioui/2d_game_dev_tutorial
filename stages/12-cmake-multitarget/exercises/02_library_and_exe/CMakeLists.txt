# Exercise 02 — a library target + an executable target
#
# Goal: compile greeter.cpp into a LIBRARY, then build an EXECUTABLE that LINKS
# against it. This is the core idea of the stage: logic goes in libraries,
# programs (and, later, tests) link the library instead of recompiling it.
#
# Build & run:
#   cmake -S . -B build && cmake --build build && ./build/app
#
# TODO:
#   1. The usual header: cmake_minimum_required + project(ex02 LANGUAGES CXX) + C++17.
#   2. add_library(greeter greeter.cpp)
#   3. add_executable(app main.cpp)
#   4. target_link_libraries(app PRIVATE greeter)
#
# (Both greeter.cpp and main.cpp #include "greeter.hpp" from the SAME directory,
#  so no include-path setup is needed yet — exercise 03 handles that.)

# TODO: your CMake code goes here.
