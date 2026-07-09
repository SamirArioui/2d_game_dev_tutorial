# Stage-level CMake shim.
#
# The real mini-project build lives in project/CMakeLists.txt. This thin wrapper exists so that
# (a) the repo-root CMakeLists.txt (which scans stages/*/CMakeLists.txt) picks this stage up, and
# (b) you can configure the whole stage directly:
#     cmake -S stages/05-cmake-and-debugging-basics -B build && cmake --build build
cmake_minimum_required(VERSION 3.16)
project(stage05_cmake_debugging LANGUAGES CXX)

add_subdirectory(project)
