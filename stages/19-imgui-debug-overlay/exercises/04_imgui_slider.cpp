// Exercise 04 — a slider that changes the scene live  (SFML + Dear ImGui)
//
// The power of a debug overlay is tuning values WITHOUT recompiling. Here a
// slider drives a circle's radius: drag it and the circle resizes immediately,
// because the slider writes straight into the `radius` variable you draw from.
//
// Build:
//   cmake -S . -B build && cmake --build build --target 04_imgui_slider
//   ./build/04_imgui_slider
//
// TODO:
//   Inside Begin/End, add:  ImGui::SliderFloat("Radius", &radius, 5.0f, 120.0f);
//   That single call both DRAWS the slider and WRITES the new value into radius.

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "Ex04 - ImGui slider");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window)) {
        return 1;
    }

    float radius = 40.0f;
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

        ImGui::Begin("Controls");
        // TODO: add the SliderFloat bound to `radius`, range 5..120.
        ImGui::End();

        sf::CircleShape circle(radius);
        circle.setFillColor(sf::Color(120, 230, 140));
        circle.setOrigin(radius, radius);
        circle.setPosition(200.0f, 200.0f);

        window.clear(sf::Color(20, 20, 30));
        window.draw(circle);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
