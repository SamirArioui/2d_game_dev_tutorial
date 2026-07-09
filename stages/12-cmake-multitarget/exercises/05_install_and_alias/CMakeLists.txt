# Exercise 05 — install rules + a namespaced ALIAS target
#
# Two finishing touches real projects add:
#   - install() rules, so `cmake --install` copies artifacts to a prefix.
#   - a namespaced ALIAS (math::math), so consumers link `math::math`. The '::'
#     makes it obvious it's a TARGET (like fmt::fmt), not a bare file name, and
#     CMake errors loudly if the target is misspelled.
#
# Build, then install into ./out:
#   cmake -S . -B build && cmake --build build
#   cmake --install build --prefix ./out
#   ./build/app
#   ls out            # -> bin/ (the app) and include/ (the header)
#
# TODO:
#   1. Header + project(ex05 LANGUAGES CXX) + C++17.
#   2. add_library(mathlib src/mathlib.cpp)
#   3. target_include_directories(mathlib PUBLIC include)
#   4. add_library(math::math ALIAS mathlib)      # namespaced alias
#   5. add_executable(app app/main.cpp)
#   6. target_link_libraries(app PRIVATE math::math)   # link through the alias
#   7. install(TARGETS app mathlib)                # note: install the REAL target
#   8. install(DIRECTORY include/ DESTINATION include)

# TODO: your CMake code goes here.
