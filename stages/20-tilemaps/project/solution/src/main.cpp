// ============================================================================
// Stage 20 mini-project — render a tile level and walk a character around it,
// colliding with solid tiles.
//
// This file is the RENDERING + INPUT layer. All the map logic it relies on
// (parsing, tile_at, solid_at) lives in the pure, unit-tested Tilemap class.
// That separation is the lesson: the interesting logic is testable; this glue
// just draws it and reads the keyboard.
//
// NOTE: this stage's bar is that the program BUILDS AND LINKS. Actual window
// behaviour is not verified here (the build machine has no display).
// ============================================================================

#include <SFML/Graphics.hpp>

#include <string>

#include "game/gmath.hpp"
#include "game/tilemap.hpp"
#include "tilemap_render.hpp"

using gmath::Vec2f;

namespace {

// A small embedded level, used if assets/level1.map can't be found (so the demo
// runs from any working directory). Same text format as the on-disk file.
constexpr const char* kFallbackMap =
    "20 15 32\n"
    "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 0 2 2 2 0 0 0 0 2 2 0 0 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 2 0 0 0 0 0 0 3 0 0 0 0 0 2 0 0 1\n"
    "1 0 0 2 0 0 0 0 0 0 3 0 0 0 0 0 2 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 0 0 0 2 2 2 2 2 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 3 3 0 0 0 0 0 0 0 0 0 3 3 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\n";

constexpr int kTileCount = 4;  // ids 0..3 exist in our tileset

// Would an axis-aligned box (top-left `pos`, size `size`) overlap ANY solid
// tile? We sample the box's four edges by walking the grid cells it covers.
// Because our character is smaller than a tile, checking the four corners is
// enough, but sampling the covered range is robust for bigger boxes too.
bool box_hits_solid(const game::Tilemap& map, Vec2f pos, Vec2f size) {
    // A tiny inset avoids false hits when the box sits flush against a boundary.
    const float e = 0.01f;
    const Vec2f corners[4] = {
        {pos.x + e, pos.y + e},                    // top-left
        {pos.x + size.x - e, pos.y + e},           // top-right
        {pos.x + e, pos.y + size.y - e},           // bottom-left
        {pos.x + size.x - e, pos.y + size.y - e},  // bottom-right
    };
    for (const Vec2f& corner : corners) {
        if (map.solid_at(corner)) {
            return true;
        }
    }
    return false;
}

}  // namespace

int main() {
    // --- load the level ----------------------------------------------------
    game::Tilemap map;
    try {
        map = game::Tilemap::from_file("assets/level1.map");
    } catch (const std::exception&) {
        map = game::Tilemap::from_text(kFallbackMap);
    }

    // --- window + rendering resources --------------------------------------
    const unsigned win_w = 800;
    const unsigned win_h = 600;
    sf::RenderWindow window(sf::VideoMode(win_w, win_h), "Stage 20 - Tilemaps");
    window.setFramerateLimit(60);

    // The tileset texture must OUTLIVE the vertex array that samples it, so we
    // keep both alive here in main's scope.
    sf::Texture tileset = game::make_tileset_texture(map.tile_size(), kTileCount);
    sf::VertexArray tiles = game::build_tilemap_vertices(map, kTileCount);
    sf::RenderStates tile_states;
    tile_states.texture = &tileset;

    // --- the character ------------------------------------------------------
    const Vec2f char_size{24.0f, 24.0f};
    Vec2f char_pos{static_cast<float>(map.tile_size()) * 2.0f,
                   static_cast<float>(map.tile_size()) * 2.0f};
    const float speed = 160.0f;  // pixels per second

    sf::RectangleShape character({char_size.x, char_size.y});
    character.setFillColor(sf::Color(230, 90, 60));

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const float dt = clock.restart().asSeconds();

        // Read desired movement from the keyboard (WASD or arrows).
        Vec2f move{0.0f, 0.0f};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            move.x -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            move.x += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            move.y -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            move.y += 1.0f;
        }

        // Resolve movement ONE AXIS AT A TIME. Moving X then Y (each reverted on
        // a collision) lets the character slide along a wall instead of sticking
        // — the standard trick for tile-based collision.
        const float step = speed * dt;

        Vec2f try_x = char_pos;
        try_x.x += move.x * step;
        if (!box_hits_solid(map, try_x, char_size)) {
            char_pos = try_x;
        }

        Vec2f try_y = char_pos;
        try_y.y += move.y * step;
        if (!box_hits_solid(map, try_y, char_size)) {
            char_pos = try_y;
        }

        // --- draw ----------------------------------------------------------
        window.clear(sf::Color(30, 30, 40));
        window.draw(tiles, tile_states);
        character.setPosition(char_pos.x, char_pos.y);
        window.draw(character);
        window.display();
    }

    return 0;
}
