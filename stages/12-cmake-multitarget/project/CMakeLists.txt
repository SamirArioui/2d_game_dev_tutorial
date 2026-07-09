# ============================================================================
# Stage 12 mini-project — multi-target CMake + FetchContent
# ============================================================================
# One project, TWO targets:
#   - `game` : a LIBRARY (add_library) holding all the reusable game logic.
#   - `app`  : an EXECUTABLE (add_executable) that links `game` + the fetched fmt.
#
# Build & run:
#   cmake -S . -B build
#   cmake --build build
#   ./build/app                 # (Windows: .\build\Debug\app.exe)
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage12_game LANGUAGES CXX)

# Language settings, set once for the whole project.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# External dependency via FetchContent — the "pip install, but at configure
# time" of the C++ world. On the FIRST configure this git-clones fmt into the
# build/ tree and builds it as part of our project. No system install needed.
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 11.0.2          # pin a version so builds are reproducible
    GIT_SHALLOW TRUE        # only fetch that one commit, not all history
)
FetchContent_MakeAvailable(fmt)   # download + add fmt's targets (fmt::fmt)

# ---------------------------------------------------------------------------
# The library target: compiled once, linked into anything that needs it.
# ---------------------------------------------------------------------------
add_library(game
    src/game_math.cpp
    src/inventory.cpp
    src/save_data.cpp
)

# PUBLIC means: `game` needs this include dir to compile ITS sources, AND anything
# that links `game` inherits it automatically. So `app` can #include "game/..."
# without repeating the path. (PRIVATE = only for building game itself.)
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ---------------------------------------------------------------------------
# The executable target: our program's entry point.
# ---------------------------------------------------------------------------
add_executable(app app/main.cpp)

# Link both our own library and the fetched fmt. PRIVATE = these are app's own
# implementation details, not re-exported to anything that might link `app`.
target_link_libraries(app PRIVATE game fmt::fmt)

# ---------------------------------------------------------------------------
# Install basics (optional): `cmake --install build --prefix ./out` would then
# copy the executable to out/bin and headers to out/include. Real projects ship
# this so others can consume the library.
# ---------------------------------------------------------------------------
install(TARGETS app game)
install(DIRECTORY include/ DESTINATION include)
