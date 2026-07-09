// Exercise 03 — your first ImGui panel  (SFML + Dear ImGui; needs a display)
//
// The five-call lifecycle of imgui-sfml:
//   Init(window)  once   -> ProcessEvent / Update / (build UI) / Render each frame
//   Shutdown()    once at the end.
// "Immediate mode" means you rebuild the UI every frame from plain calls — no
// widget objects to keep around. A button is just: "if (ImGui::Button(...)) {...}".
//
// Build:
//   cmake -S . -B build && cmake --build build --target 03_imgui_hello
//   ./build/03_imgui_hello
//
// TODO:
//   Between Begin("Hello") and End(), add:
//     - ImGui::Text("Clicked %d times", clicks);
//     - if (ImGui::Button("Click me")) { ++clicks; }

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
        // TODO: show the click count and add a button that increments `clicks`.
        (void)clicks;
        ImGui::End();

        window.clear(sf::Color(20, 20, 30));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
