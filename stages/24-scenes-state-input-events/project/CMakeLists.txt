# ============================================================================
# Stage 24 mini-project — scenes, state machine, input mapping & events.
#
#   - game_logic : the pure, window-free subsystems (state machine + scene stack). Header-only
#                  pieces (InputMap, EventBus, Scene) live under include/. Linked into BOTH the
#                  app and the tests.
#   - tests      : Catch2 over the pure subsystems. No SFML, no display -> always runnable.
#   - scenes_demo: the SFML app that wires everything together (needs a display to run).
#
# Tests only (fast, no SFML clone):
#   cmake -S . -B build -DBUILD_APP=OFF && cmake --build build
#   ctest --test-dir build --output-on-failure
#
# Everything (first run clones SFML — needs network):
#   cmake -S . -B build && cmake --build build && ./build/scenes_demo
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage24_scenes_state_input_events LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_APP "Build the SFML demo application (fetches SFML 2.6.1; needs a display to run)" ON)

include(FetchContent)

# --- pure logic: SFML-free, so it links into the tests unchanged --------------------------------
add_library(game_logic src/game_state.cpp src/scene_stack.cpp)
target_include_directories(game_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

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
    tests/test_game_state.cpp
    tests/test_input_map.cpp
    tests/test_event_bus.cpp
    tests/test_scene_stack.cpp
)
target_link_libraries(tests PRIVATE game_logic Catch2::Catch2WithMain)
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

    add_executable(scenes_demo src/main.cpp)
    target_link_libraries(scenes_demo PRIVATE game_logic sfml-graphics sfml-window sfml-system)
endif()
