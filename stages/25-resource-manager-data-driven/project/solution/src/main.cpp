// ============================================================================
// Stage 25 mini-project — a screen built entirely from DATA + a caching ResourceManager.
//                                                          (REFERENCE SOLUTION)
//
// Complete reference. The starter you implement is two levels up
// (../../include/res/resource_manager.hpp and ../../src/level_config.cpp). Build this standalone
// (it has its own CMakeLists.txt); run from this folder so assets/ resolves:
//   cmake -S project/solution -B build && cmake --build build && ./build/resource_demo
//
// Nothing about this level is hardcoded: main() reads assets/level.txt into a LevelConfig, then
// asks a ResourceManager<sf::Texture> for each entity's texture. Repeated texture paths (the whole
// grass row) are loaded ONCE and shared — that's the manager caching. Change level.txt and re-run;
// no recompile.
//
// Robustness touch used by real engines: if a texture can't be loaded we fall back to a bright
// magenta "missing texture" placeholder instead of crashing, so a typo in the data file is
// obvious on screen rather than fatal.
//
// Verified to BUILD and LINK. Run it from THIS folder (so "assets/..." resolves):
//   cmake -S . -B build && cmake --build build
//   ./build/resource_demo
// ============================================================================

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "game/level_config.hpp"
#include "res/resource_manager.hpp"

namespace {

// Build a small solid-magenta texture in memory — the classic "this asset is missing" marker.
sf::Texture make_placeholder() {
    sf::Image image;
    image.create(32, 32, sf::Color::Magenta);
    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

// Ask the manager for a texture; on failure log it and use the shared placeholder instead.
sf::Texture& texture_or_placeholder(res::ResourceManager<sf::Texture>& textures,
                                    const std::string& path, sf::Texture& placeholder) {
    try {
        return textures.get(path);
    } catch (const std::exception& e) {
        sf::err() << "[assets] " << e.what() << " — using placeholder\n";
        return placeholder;
    }
}

}  // namespace

int main() {
    // 1. DATA: load the level description from a file (falls back to a tiny built-in level if the
    //    file isn't found, so the demo still shows something).
    game::LevelConfig level;
    try {
        level = game::load_level("assets/level.txt");
    } catch (const std::exception& e) {
        sf::err() << "[level] " << e.what() << " — using a built-in fallback level\n";
        level.title = "Fallback";
        level.width = 640;
        level.height = 480;
        level.entities.push_back({"player.png", 300.0f, 220.0f});
    }

    // 2. RESOURCES: one manager, shared by every entity. Repeated paths load once.
    res::ResourceManager<sf::Texture> textures;
    sf::Texture placeholder = make_placeholder();

    // Build a sprite per entity, pulling each texture THROUGH the manager.
    std::vector<sf::Sprite> sprites;
    sprites.reserve(level.entities.size());
    for (const auto& spec : level.entities) {
        sf::Texture& tex = texture_or_placeholder(textures, spec.texture, placeholder);
        sf::Sprite sprite(tex);
        sprite.setPosition(spec.x, spec.y);
        sprites.push_back(sprite);
    }

    // Proof of caching: e.g. 8 entities but far fewer UNIQUE textures actually loaded.
    sf::err() << "[assets] " << level.entities.size() << " entities, " << textures.size()
              << " unique textures loaded (cached)\n";

    const auto width = static_cast<unsigned>(level.width > 0 ? level.width : 640);
    const auto height = static_cast<unsigned>(level.height > 0 ? level.height : 480);
    sf::RenderWindow window(sf::VideoMode(width, height), "Stage 25 — " + level.title);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(40, 40, 55));
        for (const auto& sprite : sprites) {
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}
