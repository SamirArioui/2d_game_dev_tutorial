# ============================================================================
# Stage 19 mini-project — Pong + Dear ImGui debug overlay.  (REFERENCE SOLUTION)
# ============================================================================
# This is the complete reference: the debug helpers in src/tunables.cpp are
# fully implemented (and the reused pong rules are complete), so `ctest` here is
# GREEN. The STARTER you fill in is one level up (../src/tunables.cpp) — the pong
# core is carried over intact from stage 18; only the debug helpers are yours.
# Try it yourself first, then compare.
#
# Self-contained and NOT wired into any parent/course CMake; build it on its own:
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ctest --test-dir build --output-on-failure     # -> all green
# ============================================================================
#
# Targets:
#   - pong_core   : SFML-FREE logic (pong rules + debug helpers). Tested.
#   - pong_tests  : Catch2 runner over pong_core -> `ctest`, headless.
#   - pong_imgui  : the app. Links SFML + Dear ImGui + imgui-sfml.
#
# Three dependencies are fetched. The order matters:
#   1. SFML 2.6.1            (imgui-sfml renders through it)
#   2. Dear ImGui (sources)  — has no CMakeLists, so FetchContent only DOWNLOADS
#                              it; imgui-sfml compiles it via IMGUI_DIR.
#   3. imgui-sfml v2.6       — the SFML backend for ImGui, matched to SFML 2.6.
#
# Build (first configure clones all three + Catch2 -> needs network):
#   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#   cmake --build build
#   ctest --test-dir build --output-on-failure     # green
#   ./build/pong_imgui                               # needs a display
#
# Fast logic-only loop (does NOT compile SFML/ImGui):
#   cmake --build build --target pong_tests && ctest --test-dir build
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage19_imgui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# --- 1) SFML 2.6.1 ----------------------------------------------------------
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)  # stage 19 focuses on the overlay
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

# --- 2) Dear ImGui (sources only) -------------------------------------------
# ImGui has no top-level CMakeLists.txt, so FetchContent_MakeAvailable just
# populates it (no add_subdirectory). We hand its path to imgui-sfml below.
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.90.9
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

# --- 3) imgui-sfml v2.6 (the SFML backend) ----------------------------------
# Tell it NOT to find_package(SFML) — reuse the sfml-* targets we just fetched.
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_SFML_IMGUI_DEMO OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v2.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ImGui-SFML)

# --- Catch2 -----------------------------------------------------------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

# --- the SFML-free logic (pong rules + debug helpers) -----------------------
add_library(pong_core
    src/pong.cpp
    src/aabb.cpp
    src/tunables.cpp
)
target_include_directories(pong_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- tests: pong_core + Catch2, no SFML/ImGui -------------------------------
enable_testing()
add_executable(pong_tests
    tests/test_pong.cpp
    tests/test_collision.cpp
    tests/test_tunables.cpp
)
target_link_libraries(pong_tests PRIVATE pong_core Catch2::Catch2WithMain)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(pong_tests)

# --- the app: links our logic + imgui-sfml (which pulls in ImGui + SFML) ----
add_executable(pong_imgui src/main.cpp)
target_link_libraries(pong_imgui PRIVATE pong_core ImGui-SFML::ImGui-SFML)
