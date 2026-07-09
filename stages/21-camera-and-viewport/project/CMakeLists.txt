# ============================================================================
# Stage 21 mini-project — a scrolling camera over a tilemap, with parallax + HUD.
#
# Same three-target shape as stage 20:
#   camera_core  : the PURE camera maths + the (carried) tilemap logic. No SFML.
#   camera_demo  : the SFML app (sf::View follow/clamp, parallax, HUD).
#   camera_tests : Catch2 tests over the pure camera maths. No SFML.
#
# Build & run:
#   cmake -S . -B build          # first run clones SFML 2.6.1 + Catch2 v3.7.1
#   cmake --build build
#   ctest --test-dir build --output-on-failure   # camera maths tests -> green
#   ./build/camera_demo          # run from project/ so it finds assets/level2.map
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage21_camera LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- pure logic library: camera maths + tilemap, linked by app AND tests ----
add_library(camera_core
    src/camera.cpp
    src/tilemap.cpp
)
target_include_directories(camera_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

# --- the SFML app (build+link is the bar) -----------------------------------
add_executable(camera_demo
    src/main.cpp
    src/tilemap_render.cpp
)
target_link_libraries(camera_demo PRIVATE camera_core sfml-graphics sfml-window sfml-system)

# --- the test runner (pure maths, no SFML) ----------------------------------
enable_testing()
add_executable(camera_tests tests/test_camera.cpp)
target_link_libraries(camera_tests PRIVATE camera_core Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(camera_tests)
