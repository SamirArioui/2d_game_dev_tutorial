# ============================================================================
# Stage 12 mini-project — multi-target CMake + FetchContent   (YOUR TASK)
# ============================================================================
# This is the STARTER. The LESSON of this stage is the CMake structure itself,
# so the C++ sources under src/ and app/ are ALREADY COMPLETE — your job is to
# write the build description below.
#
# Fill in every TODO(stage 12) so that:
#   - the reusable game logic (src/*.cpp) compiles into a `game` LIBRARY,
#   - app/main.cpp compiles into an `app` EXECUTABLE,
#   - `app` links `game` AND the fetched `fmt`,
#   - `fmt` is pulled from GitHub with FetchContent.
#
# A complete, working reference is in ./solution/CMakeLists.txt — try it
# yourself first, then compare.
#
# Build & run (works once you have completed the TODOs):
#   cmake -S . -B build          # first run clones fmt into build/_deps (needs network)
#   cmake --build build          # builds libgame.a, fmt, then the app
#   ./build/app                  # (Windows: .\build\Debug\app.exe)
#
# As shipped, this file configures but declares NO targets, so `cmake --build`
# produces nothing and ./build/app does not exist yet — that is the work below.
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(stage12_game LANGUAGES CXX)

# Language settings, set once for the whole project.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# 1. External dependency via FetchContent — "pip install, but at configure
#    time". On the FIRST configure this git-clones fmt into the build/ tree.
# ---------------------------------------------------------------------------
# TODO(stage 12): include(FetchContent), then FetchContent_Declare(fmt ...) with
#   GIT_REPOSITORY https://github.com/fmtlib/fmt
#   GIT_TAG        11.0.2      (pin a version so builds are reproducible)
#   GIT_SHALLOW    TRUE        (only fetch that one commit)
# and finish with FetchContent_MakeAvailable(fmt) to add the `fmt::fmt` target.

# ---------------------------------------------------------------------------
# 2. The library target: compiled once, linked into anything that needs it.
# ---------------------------------------------------------------------------
# TODO(stage 12): add_library(game ...) from the three library sources:
#   src/game_math.cpp  src/inventory.cpp  src/save_data.cpp

# TODO(stage 12): target_include_directories(game PUBLIC ...) pointing at
#   ${CMAKE_CURRENT_SOURCE_DIR}/include so `game` compiles AND everything that
#   links it (the app) inherits the include path and can #include "game/...".
#   PUBLIC = needed to build game itself AND re-exported to consumers.

# ---------------------------------------------------------------------------
# 3. The executable target: our program's entry point (has main()).
# ---------------------------------------------------------------------------
# TODO(stage 12): add_executable(app app/main.cpp)

# TODO(stage 12): target_link_libraries(app PRIVATE game fmt::fmt) so the app
#   can call the library's functions and fmt::print / fmt::format. PRIVATE =
#   these are app's own implementation details.

# ---------------------------------------------------------------------------
# 4. Install basics (optional).
# ---------------------------------------------------------------------------
# TODO(stage 12, optional): once the targets above exist, add
#   install(TARGETS app game)
#   install(DIRECTORY include/ DESTINATION include)
