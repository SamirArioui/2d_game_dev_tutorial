// Exercise 03 — solution  (SFML + Dear ImGui)
#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 200), "Ex03 - ImGui hello");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window)) {
        return 1;
    }

    int clicks = 0;
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        ImGui::SFML::Update(window, clock.restart());

        ImGui::Begin("Hello");
        ImGui::Text("Clicked %d times", clicks);
        if (ImGui::Button("Click me")) {  // true only on the frame it's clicked
            ++clicks;
        }
        ImGui::End();

        window.clear(sf::Color(20, 20, 30));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
