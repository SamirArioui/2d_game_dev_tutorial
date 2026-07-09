# Stage 06 — Pointers, references & memory debugging
#
# This is the first stage where the build system itself is part of the lesson: it exposes an
# option to turn on AddressSanitizer (ASan) + UndefinedBehaviorSanitizer (UBSan), the runtime
# memory-error detectors this stage teaches. They are ON by default here.
#
#   cmake -S . -B build                  # ASan + UBSan ON (default for this stage)
#   cmake --build build
#   ./build/swap_and_list                # the mini-project — runs clean under ASan
#   ./build/ex_06_use_after_free_bug     # the planted bug -> ASan prints a report and aborts
#
# Turn the sanitizers off for a plain build:
#   cmake -S . -B build -DENABLE_ASAN=OFF && cmake --build build
cmake_minimum_required(VERSION 3.16)
project(stage06_pointers_references_asan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- AddressSanitizer / UBSan toggle ---------------------------------------------------
# -fsanitize=address      : detect use-after-free, heap-buffer-overflow, double-free, ...
# -fsanitize=undefined    : detect undefined behaviour (signed overflow, bad shifts, ...)
# -fno-omit-frame-pointer : keep frame pointers so ASan's stack traces are readable
# -g                      : debug info so the report can name files and line numbers
option(ENABLE_ASAN "Build with AddressSanitizer + UndefinedBehaviorSanitizer" ON)
if(ENABLE_ASAN)
    message(STATUS "stage06: AddressSanitizer + UBSan ENABLED")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()

add_compile_options(-Wall -Wextra)

# --- mini-project ----------------------------------------------------------------------
add_executable(swap_and_list project/src/main.cpp)

# --- exercise solutions (one runnable target each) -------------------------------------
file(GLOB exercise_solutions CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/exercises/*.solution.cpp")
foreach(sol ${exercise_solutions})
    get_filename_component(fname "${sol}" NAME)
    string(REPLACE ".solution.cpp" "" stem "${fname}")
    add_executable("ex_${stem}" "${sol}")
endforeach()

# The planted-bug program is the buggy STARTER (not the solution). It is built as its own
# target so you can run it under ASan and watch the detector catch the use-after-free.
add_executable(ex_06_use_after_free_bug exercises/06_use_after_free.cpp)
