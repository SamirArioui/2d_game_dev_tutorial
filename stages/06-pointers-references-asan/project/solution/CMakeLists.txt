# Stage 06 mini-project — REFERENCE SOLUTION (standalone build)
#
# This is the complete reference for the mini-project. It is self-contained and NOT wired into
# the stage's parent CMakeLists.txt — build it on its own to compare against your own version:
#
#   cmake -S project/solution -B build                 # ASan + UBSan ON (default, like the stage)
#   cmake --build build
#   ./build/swap_and_list                              # runs clean under ASan
#
# Turn the sanitizers off for a plain build:
#   cmake -S project/solution -B build -DENABLE_ASAN=OFF && cmake --build build
cmake_minimum_required(VERSION 3.16)
project(stage06_mini_project_solution LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- AddressSanitizer / UBSan toggle (mirrors the stage; ON by default) ----------------
option(ENABLE_ASAN "Build with AddressSanitizer + UndefinedBehaviorSanitizer" ON)
if(ENABLE_ASAN)
    message(STATUS "stage06 solution: AddressSanitizer + UBSan ENABLED")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()

add_compile_options(-Wall -Wextra)

add_executable(swap_and_list src/main.cpp)
