# Stage 05 mini-project — the stage-04 multi-file math program, now built by CMake.
#
# In stage 04 you typed this by hand:
#   clang++ -std=c++17 -Wall -Wextra src/main.cpp src/mathutils.cpp src/player.cpp -o mathgame
# This file describes the SAME build declaratively; CMake generates and runs those commands.
#
# Configure, build, run (from this folder):
#   cmake -S . -B build          # configure: read this file, generate a build system in build/
#   cmake --build build          # compile every source, then link -> build/mathgame
#   ./build/mathgame             # run  (Windows multi-config: ./build/Debug/mathgame.exe)
#
# Debug (symbols for lldb, asserts on) vs Release (optimized, asserts off):
#   cmake -S . -B build     -DCMAKE_BUILD_TYPE=Debug
#   cmake -S . -B build-rel -DCMAKE_BUILD_TYPE=Release

# Refuse ancient CMake whose default behaviors differ from what this file assumes.
cmake_minimum_required(VERSION 3.16)

# Name the project and declare the language. This must appear before any targets.
project(mathgame LANGUAGES CXX)

# Compile everything as C++17 (the -std=c++17 flag, applied to every file, no extensions).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# If the user didn't choose a build type, default to Debug so lldb has symbols out of the box.
# (Only meaningful for single-config generators like Makefiles/Ninja; ignored by Visual Studio.)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Emit compile_commands.json so clang-tidy and editors know each file's exact flags.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# One executable target built from all three translation units. CMake works out the
# compile-each-then-link dance (the thing you did by hand in stage 04).
add_executable(mathgame
    src/main.cpp
    src/mathutils.cpp
    src/player.cpp
)

# The -Wall -Wextra you used to pass by hand — but only for GCC/Clang (MSVC spells them
# differently). PRIVATE = these flags apply when building THIS target, and don't leak to others.
if(NOT MSVC)
    target_compile_options(mathgame PRIVATE -Wall -Wextra)
endif()
