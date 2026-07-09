# Exercise 04 — pull an external dependency with FetchContent
#
# fmt is NOT installed on your system; CMake will git-clone and build it as part
# of your configure step. This is the C++ answer to `pip install` — but it
# happens at build-configure time and pins an exact version for reproducibility.
#
# Build & run (the FIRST configure needs network to clone fmt):
#   cmake -S . -B build && cmake --build build && ./build/app
#
# TODO:
#   1. Header + project(ex04 LANGUAGES CXX) + C++17.
#   2. include(FetchContent)
#   3. FetchContent_Declare(
#          fmt
#          GIT_REPOSITORY https://github.com/fmtlib/fmt
#          GIT_TAG 11.0.2
#          GIT_SHALLOW TRUE
#      )
#   4. FetchContent_MakeAvailable(fmt)
#   5. add_executable(app main.cpp)
#   6. target_link_libraries(app PRIVATE fmt::fmt)

# TODO: your CMake code goes here.
