# Exercise 03 — PUBLIC include directories
#
# The header now lives in include/, away from the .cpp files in src/ and app/.
# Both the library's own source AND the app that uses it need to find it. The
# clean way is one line on the library, marked PUBLIC so it propagates.
#
# Build & run:
#   cmake -S . -B build && cmake --build build && ./build/app
#
# TODO:
#   1. Header + project(ex03 LANGUAGES CXX) + C++17.
#   2. add_library(mathlib src/mathlib.cpp)
#   3. target_include_directories(mathlib PUBLIC include)
#        PUBLIC = mathlib needs include/ to build ITS sources, AND every target
#        that links mathlib inherits include/ automatically. (PRIVATE would keep
#        it to mathlib only, and the app's #include would fail to find the header.)
#   4. add_executable(app app/main.cpp)
#   5. target_link_libraries(app PRIVATE mathlib)

# TODO: your CMake code goes here.
