# ============================================================================
# Stage 23 mini-project — Pong as a component model.                (REFERENCE SOLUTION)
# ============================================================================
# This is the complete reference build: src/systems.cpp and include/ecs/entity.hpp are fully
# implemented here, so `ctest` in this folder is GREEN. The STARTER you fill in is one level up
# (../src/systems.cpp and ../include/ecs/entity.hpp); the tests and this CMakeLists are shared.
#
# One project, three moving parts:
#   - ecs_logic : the pure, window-free game logic (systems.cpp). Linked into BOTH the app and the
#                 tests, which is exactly why we keep it SFML-free.
#   - tests     : a Catch2 runner over ecs_logic. No SFML, no display -> always buildable/runnable.
#   - pong_ecs  : the SFML demo. Needs a display to RUN, so it is optional (BUILD_APP).
#
# Build the tests only (fast, no display, no SFML clone):
#   cmake -S . -B build -DBUILD_APP=OFF
#   cmake --build build
#   ctest --test-dir build --output-on-failure
#
# Build everything including the SFML demo (first run clones SFML — needs network):
#   cmake -S . -B build
#   cmake --build build
#   ./build/pong_ecs
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage23_component_model LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_APP "Build the SFML demo application (fetches SFML 2.6.1; needs a display to run)" ON)

include(FetchContent)

# --- pure game logic: header includes live under include/, one .cpp of systems ------------------
add_library(ecs_logic src/systems.cpp)
target_include_directories(ecs_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Catch2 test runner (always built; the payoff of keeping logic window-free) -----------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

enable_testing()
add_executable(tests tests/test_ecs.cpp)
target_link_libraries(tests PRIVATE ecs_logic Catch2::Catch2WithMain)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tests)

# --- the SFML demo (optional) -------------------------------------------------------------------
if(BUILD_APP)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
        GIT_SHALLOW TRUE
    )
    # Pong needs only graphics/window/system — skip the parts we don't use to build faster.
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SFML)

    add_executable(pong_ecs src/main.cpp)
    target_link_libraries(pong_ecs PRIVATE ecs_logic sfml-graphics sfml-window sfml-system)
endif()
