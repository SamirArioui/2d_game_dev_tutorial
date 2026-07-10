// ============================================================================
// Stage 26 mini-project — Pong, ported onto libengine.  (REFERENCE SOLUTION)
//
// The starter you fill in is two levels up at ../../src/. This is the complete,
// working version.
//
// Look how SMALL main() is now. There is no window creation, no event loop, no
// delta-time clock, no clear/draw/display dance — the ENGINE owns all of that.
// main() just: configure the app, push the first scene, and run(). Everything
// game-specific lives in PongScene; everything loop-specific lives in the engine.
//
// That is the deliverable of this stage: the reusable loop is written ONCE, and
// the stage-27 capstone will reuse this exact Application with a different scene.
//
// Build & run (from stages/26-extract-the-engine/project):
//   cmake -S . -B build            # first configure clones SFML + Catch2
//   cmake --build build
//   ./build/pong                   # opens a window (needs a display)
// ============================================================================

#include <memory>

#include "eng/application.hpp"
#include "pong_scene.hpp"

int main() {
    eng::AppConfig config;
    config.width = 800;
    config.height = 600;
    config.title = "Pong on libengine (stage 26)";
    config.clear_color = sf::Color(15, 15, 25);

    eng::Application app(config);

    // The scene's court matches the window. (A single source of truth would be
    // even better — an exercise suggests wiring court size from the window.)
    pong::Config court;
    court.court_w = static_cast<float>(config.width);
    court.court_h = static_cast<float>(config.height);

    app.scenes().push(std::make_unique<pong::PongScene>(court));
    app.run();
    return 0;
}
