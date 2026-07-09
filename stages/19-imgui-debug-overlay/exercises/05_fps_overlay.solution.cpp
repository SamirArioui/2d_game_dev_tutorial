// Exercise 05 — solution  (SFML + Dear ImGui)
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

        const float fps = (dt > 0.0f) ? 1.0f / dt : 0.0f;
        smoothed_fps = smoothed_fps * 0.9f + fps * 0.1f;  // exponential moving average

        ImGui::Begin("Profiler");
        ImGui::Text("FPS: %.1f", static_cast<double>(smoothed_fps));
        ImGui::End();

        window.clear(sf::Color(20, 20, 30));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
