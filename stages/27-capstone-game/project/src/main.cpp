// ============================================================================
// Stage 27 capstone — a top-down explorer on libengine.
//
// Same shape as the stage-26 Pong main(): configure the app, push ONE scene,
// run(). The engine and its Application loop are IDENTICAL to stage 26 — this is
// the proof the engine is genuinely reusable. All the new work went into the
// game (ExploreScene + the pure game logic), not the loop.
//
// Build & run (from stages/27-capstone-game/project):
//   cmake -S . -B build            # first configure clones SFML + Catch2
//   cmake --build build
//   ./build/capstone               # opens a window (needs a display)
// ============================================================================

#include <memory>
#include <string>

#include "eng/application.hpp"
#include "explore_scene.hpp"

#ifndef CAPSTONE_ASSET_DIR
#define CAPSTONE_ASSET_DIR "assets"  // fallback; CMake normally sets an absolute path
#endif

int main() {
    eng::AppConfig config;
    config.width = 640;
    config.height = 480;
    config.title = "Capstone: Top-Down Explorer (stage 27)";
    config.clear_color = sf::Color(18, 20, 26);

    eng::Application app(config);

    const gmath::Vec2f view_size{static_cast<float>(config.width),
                                 static_cast<float>(config.height)};

    app.scenes().push(std::make_unique<capstone::ExploreScene>(
        view_size, std::string(CAPSTONE_ASSET_DIR), std::string("capstone_save.txt")));

    app.run();
    return 0;
}
