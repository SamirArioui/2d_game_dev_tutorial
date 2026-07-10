# ============================================================================
# Stage 25 mini-project — caching ResourceManager + data-driven level config.  (REFERENCE SOLUTION)
# ============================================================================
# This is the complete reference build: ResourceManager<T> (include/res/) and the level parser
# (src/level_config.cpp) are fully implemented here, so `ctest` in this folder is GREEN. The
# STARTER you fill in is one level up (../include/res/resource_manager.hpp and
# ../src/level_config.cpp); the tests, assets and this CMakeLists are shared.
#
#   - level_logic : the pure config parser (level_config.cpp). ResourceManager<T> is a header-only
#                   template (include/res/). Both are SFML-free and link into the tests.
#   - tests       : Catch2 over the manager's caching and the parser. No SFML, no display.
#   - resource_demo: the SFML app that loads assets/level.txt and draws it (needs a display to run).
#
# Tests only (fast):
#   cmake -S . -B build -DBUILD_APP=OFF && cmake --build build
#   ctest --test-dir build --output-on-failure
#
# Everything (first run clones SFML — needs network); run from THIS folder so assets/ resolves:
#   cmake -S . -B build && cmake --build build && ./build/resource_demo
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage25_resource_manager_data_driven LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_APP "Build the SFML demo application (fetches SFML 2.6.1; needs a display to run)" ON)

include(FetchContent)

# --- pure logic: the config parser; ResourceManager<T> is header-only under include/ ------------
add_library(level_logic src/level_config.cpp)
target_include_directories(level_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Catch2 test runner (always built) ----------------------------------------------------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

enable_testing()
add_executable(tests
    tests/test_resource_manager.cpp
    tests/test_level_config.cpp
)
target_link_libraries(tests PRIVATE level_logic Catch2::Catch2WithMain)
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
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SFML)

    add_executable(resource_demo src/main.cpp)
    target_link_libraries(resource_demo PRIVATE level_logic sfml-graphics sfml-window sfml-system)
endif()
