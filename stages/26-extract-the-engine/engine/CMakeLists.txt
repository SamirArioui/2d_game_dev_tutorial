# ============================================================================
# libengine — the reusable engine as a STATIC library target.
#
# This CMakeLists defines ONE target: a static library called `engine`. It does
# NOT fetch SFML itself; it assumes the PARENT project has already made SFML's
# targets available (via FetchContent) and then done add_subdirectory() on this
# folder. That mirrors how real libraries are consumed: the top-level build owns
# the dependency graph; a library just declares what it needs.
#
# STATIC vs DYNAMIC (the lesson):
#   - A STATIC library (.a/.lib) is COPIED into each executable that links it at
#     link time. One self-contained binary, no "where's the .dll?" at runtime.
#   - A DYNAMIC/shared library (.so/.dll/.dylib) is loaded at RUN time and shared
#     between programs — smaller binaries, but you must ship the library and the
#     loader must find it.
# We pick STATIC here because it is the simplest thing that works for a course
# engine: link it and you are done, nothing to deploy alongside the game.
# ============================================================================

# Collect the compiled units. The pure headers (math, collision, tilemap query,
# scene, resource_manager) are header-only; these .cpp files are the parts that
# actually touch SFML and therefore need compiling.
add_library(engine STATIC
    src/application.cpp
    src/audio.cpp
    src/camera.cpp
    src/font.cpp
    src/particles.cpp
    src/textures.cpp
    src/tile_renderer.cpp
    src/tilemap.cpp
)

# PUBLIC include dir: `engine`'s own sources need it, AND anything that links
# `engine` inherits it, so games just #include "eng/engine.hpp".
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(engine PUBLIC cxx_std_17)

# Link SFML PUBLICLY: the engine's headers expose SFML types (sf::RenderWindow,
# sf::Texture, ...), so every consumer of `engine` also needs SFML. PUBLIC makes
# CMake propagate the SFML link + include flags to them automatically.
target_link_libraries(engine PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# --- The engine's own unit tests ---------------------------------------------
# Only built when the parent project has fetched Catch2 (every stage project
# does). Like the game tests, they link Catch2 alone — no SFML: they cover the
# PURE core (scene stack, resource manager), which is header-only.
if(TARGET Catch2::Catch2WithMain)
    enable_testing()
    add_executable(engine_tests tests/test_engine_core.cpp)
    target_include_directories(engine_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_compile_features(engine_tests PRIVATE cxx_std_17)
    target_link_libraries(engine_tests PRIVATE Catch2::Catch2WithMain)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(engine_tests)
endif()
