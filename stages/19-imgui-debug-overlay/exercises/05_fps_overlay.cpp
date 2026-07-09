// Exercise 05 — an FPS readout in the overlay  (SFML + Dear ImGui)
//
// The most useful debug widget: how fast is the frame loop running? Each frame
// has a delta time `dt` (seconds). Instantaneous FPS is 1/dt, but that jitters,
// so we smooth it toward a running value. Then we print it with ImGui::Text.
//
// Build:
//   cmake -S . -B build && cmake --build build --target 05_fps_overlay
//   ./build/05_fps_overlay
//
// TODO:
//   1. Compute instantaneous fps from dt (guard dt <= 0).
//   2. Smooth it:  smoothed = smoothed*0.9f + fps*0.1f;  (an exponential average)
//   3. ImGui::Text("FPS: %.1f", smoothed);

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(300, 120), "Ex05 - FPS overlay");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window)) {
        return 1;
    }

    float smoothed_fps = 0.0f;
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const sf::Time elapsed = clock.restart();
        const float dt = elapsed.asSeconds();
        ImGui::SFML::Update(window, elapsed);

        // TODO: compute fps from dt, update smoothed_fps, and show it.
        (void)dt;

        ImGui::Begin("Profiler");
        // TODO: ImGui::Text("FPS: %.1f", smoothed_fps);
        (void)smoothed_fps;
        ImGui::End();

        window.clear(sf::Color(20, 20, 30));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
