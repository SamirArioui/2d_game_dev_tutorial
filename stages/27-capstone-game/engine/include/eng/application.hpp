#pragma once

// ============================================================================
// eng::Application — the piece that OWNS the main loop.
//
// This is the heart of the "extract the engine" lesson. In stages 14-18 the
// GAME owned the loop: your main() had the `while (window.isOpen())`, polled
// events, computed dt, updated, drew. Every game re-wrote that boilerplate.
//
// Here the ENGINE owns the loop. `Application::run()` is the `while` loop, and
// it CALLS BACK into whatever Scene is on top of the stack:
//
//        game code (a Scene)                 engine (Application)
//        -------------------                 --------------------
//        "the game owns the loop"            "the engine owns the loop"
//        while (open) {                      run() {
//            poll(); update(); draw();  -->      while (open) {
//        }                                           poll -> scene.handle_input()
//                                                    dt   -> scene.update(dt)
//                                                            scene.render(window)
//                                                    }
//                                                }
//
// This flip is called INVERSION OF CONTROL: the framework calls you, you don't
// call it. Python devs have felt it before — a Flask view or a pytest test
// function is called BY the framework; you never write Flask's request loop.
// ============================================================================

#include <string>

#include <SFML/Graphics.hpp>

#include "eng/scene.hpp"

namespace eng {

// Window + loop configuration, filled in by the game before constructing the app.
struct AppConfig {
    unsigned width{800};
    unsigned height{600};
    std::string title{"eng::Application"};
    unsigned fps_limit{60};                 // 0 = unlimited
    sf::Color clear_color{sf::Color::Black};
};

class Application {
public:
    explicit Application(const AppConfig& config);

    // The game pushes its first scene here before calling run().
    SceneStack& scenes() { return scenes_; }
    const SceneStack& scenes() const { return scenes_; }

    // Some scenes need the window (to size a view, read pixel dimensions, ...).
    sf::RenderWindow& window() { return window_; }

    // Runs until the window is closed or the scene stack empties. Blocks.
    void run();

private:
    sf::RenderWindow window_;
    SceneStack scenes_;
    sf::Color clear_color_;
};

}  // namespace eng
