# Exercise 01 — Write a minimal CMakeLists.txt
#
# In stage 04 you typed the compile+link command by hand. CMake's job is to generate and run
# those commands for you from a short description of your project. Fill in the three TODOs so
# this configures and builds. Compare with CMakeLists.solution.txt when you're done.
#
# Build & run (from THIS folder):
#   cmake -S . -B build          # configure: read this file, generate a build system in build/
#   cmake --build build          # compile + link
#   ./build/hello                # run (Windows multi-config: ./build/Debug/hello.exe)
#
# Tip: after a successful configure, `cmake --build build` is all you rerun as you edit code.

# TODO 1: require at least CMake 3.16 so behavior is predictable.
#   cmake_minimum_required(VERSION 3.16)

# TODO 2: name the project and declare it's C++ (LANGUAGES CXX).
#   project(hello_cmake LANGUAGES CXX)

# Compile as C++17 (the equivalent of the -std=c++17 flag you passed by hand).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO 3: build an executable named `hello` from main.cpp.
#   add_executable(hello main.cpp)
