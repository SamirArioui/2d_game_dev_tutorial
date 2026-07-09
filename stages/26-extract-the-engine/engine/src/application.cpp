#include "eng/application.hpp"

namespace eng {

Application::Application(const AppConfig& config)
    : window_(sf::VideoMode(config.width, config.height), config.title),
      clear_color_(config.clear_color) {
    // A frame limit keeps a trivial scene from spinning the CPU/GPU at 5000 fps.
    window_.setFramerateLimit(config.fps_limit);
}

void Application::run() {
    // One clock, restarted every frame; asSeconds() gives us delta time (dt),
    // the wall-clock seconds the previous frame took. Multiplying movement by dt
    // makes the game run at the same SPEED regardless of frame rate.
    sf::Clock clock;

    while (window_.isOpen()) {
        // 1) INPUT — drain the OS event queue and forward each event to the
        //    current scene. The engine handles the universal "window closed"
        //    case itself so every game doesn't have to.
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
            }
            scenes_.handle_input(event);
        }

        // 2) UPDATE — advance the current scene by dt.
        const float dt = clock.restart().asSeconds();
        scenes_.update(dt);

        // A scene can pop itself (e.g. "quit" from a menu); if nothing is left
        // there is nothing to draw, so we shut down cleanly.
        if (scenes_.empty()) {
            window_.close();
            break;
        }

        // 3) RENDER — clear, let the scene draw, present the frame.
        window_.clear(clear_color_);
        scenes_.render(window_);
        window_.display();
    }
}

}  // namespace eng
