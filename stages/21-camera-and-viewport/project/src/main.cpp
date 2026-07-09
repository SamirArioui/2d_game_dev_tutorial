// ============================================================================
// Stage 21 mini-project — a camera that follows a character across a tilemap,
// with a parallax background layer and a screen-space HUD.
//
// Three coordinate spaces are juggled here, and keeping them straight is the
// whole lesson:
//   WORLD  — where the level and the player live (pixels, can exceed the window)
//   SCREEN — window pixels (0..800, 0..600)
//   VIEW   — the rectangle of WORLD that sf::View maps onto the SCREEN
//
// The camera work (follow + clamp), the world<->screen conversion, and the
// parallax offset are all in the PURE, unit-tested game::camera functions. This
// file is the SFML glue: it owns the sf::View and does the drawing.
//
// NOTE: the bar for this stage is BUILD + LINK. The build machine has no
// display, so the on-screen result is unverified-by-execution.
// ============================================================================

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <random>

#include "game/camera.hpp"
#include "game/gmath.hpp"
#include "game/tilemap.hpp"
#include "tilemap_render.hpp"

using gmath::Vec2f;

namespace {

constexpr int kTileCount = 4;

constexpr const char* kFallbackMap =
    "12 8 32\n"
    "1 1 1 1 1 1 1 1 1 1 1 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 2 0 0 0 0 3 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 0 2 2 0 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 0 0 1\n"
    "1 1 1 1 1 1 1 1 1 1 1 1\n";

// The four-corner box-vs-solid test carried over from stage 20.
bool box_hits_solid(const game::Tilemap& map, Vec2f pos, Vec2f size) {
    const float e = 0.01f;
    const Vec2f corners[4] = {
        {pos.x + e, pos.y + e},
        {pos.x + size.x - e, pos.y + e},
        {pos.x + e, pos.y + size.y - e},
        {pos.x + size.x - e, pos.y + size.y - e},
    };
    for (const Vec2f& c : corners) {
        if (map.solid_at(c)) {
            return true;
        }
    }
    return false;
}

// A procedural starfield used as the parallax background — random dots on a
// dark texture. Generated with sf::Image so there are no binary asset files.
sf::Texture make_starfield(unsigned size, unsigned star_count) {
    sf::Image image;
    image.create(size, size, sf::Color(12, 12, 24));
    std::mt19937 rng(1234);  // fixed seed -> the same sky every run
    std::uniform_int_distribution<unsigned> pos(0, size - 1);
    std::uniform_int_distribution<int> bright(120, 255);
    for (unsigned i = 0; i < star_count; ++i) {
        const int b = bright(rng);
        image.setPixel(pos(rng), pos(rng),
                       sf::Color(static_cast<sf::Uint8>(b), static_cast<sf::Uint8>(b),
                                 static_cast<sf::Uint8>(b)));
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    texture.setRepeated(true);  // so a small tile fills any screen size
    return texture;
}

}  // namespace

int main() {
    // --- level --------------------------------------------------------------
    game::Tilemap map;
    try {
        map = game::Tilemap::from_file("assets/level2.map");
    } catch (const std::exception&) {
        map = game::Tilemap::from_text(kFallbackMap);
    }
    const Vec2f level_size{static_cast<float>(map.width() * map.tile_size()),
                           static_cast<float>(map.height() * map.tile_size())};

    // --- window + views -----------------------------------------------------
    const unsigned win_w = 800;
    const unsigned win_h = 600;
    const Vec2f window_size{static_cast<float>(win_w), static_cast<float>(win_h)};
    sf::RenderWindow window(sf::VideoMode(win_w, win_h), "Stage 21 - Camera & viewport");
    window.setFramerateLimit(60);

    // The GAME view shows a window-sized rectangle of the world and scrolls.
    sf::View game_view(sf::FloatRect(0.0f, 0.0f, window_size.x, window_size.y));
    // The DEFAULT view is fixed to the window — we draw the HUD in it.
    const sf::View hud_view = window.getDefaultView();

    // --- rendering resources ------------------------------------------------
    sf::Texture tileset = game::make_tileset_texture(map.tile_size(), kTileCount);
    sf::VertexArray tiles = game::build_tilemap_vertices(map, kTileCount);
    sf::RenderStates tile_states;
    tile_states.texture = &tileset;

    sf::Texture stars = make_starfield(256, 400);
    sf::Sprite background(stars);
    const float parallax_factor = 0.3f;  // background scrolls at 30% of the camera

    // --- player + HUD -------------------------------------------------------
    const Vec2f player_size{24.0f, 24.0f};
    Vec2f player_pos{level_size.x * 0.5f, level_size.y * 0.5f};
    const float speed = 220.0f;
    sf::RectangleShape player({player_size.x, player_size.y});
    player.setFillColor(sf::Color(230, 90, 60));

    // A small minimap panel in the HUD, top-right corner.
    const Vec2f minimap_size{160.0f, 80.0f};
    const Vec2f minimap_origin{window_size.x - minimap_size.x - 12.0f, 12.0f};
    sf::RectangleShape minimap({minimap_size.x, minimap_size.y});
    minimap.setPosition(minimap_origin.x, minimap_origin.y);
    minimap.setFillColor(sf::Color(0, 0, 0, 140));
    minimap.setOutlineColor(sf::Color(200, 200, 200));
    minimap.setOutlineThickness(1.0f);
    sf::CircleShape minimap_dot(3.0f);
    minimap_dot.setOrigin(3.0f, 3.0f);
    minimap_dot.setFillColor(sf::Color(230, 90, 60));

    // A marker dropped in WORLD space wherever you last clicked.
    sf::CircleShape click_marker(6.0f);
    click_marker.setOrigin(6.0f, 6.0f);
    click_marker.setFillColor(sf::Color(90, 200, 255));
    bool has_marker = false;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Click-to-world: the click arrives in SCREEN pixels; convert it to
            // WORLD space using the game view. SFML's mapPixelToCoords does the
            // same maths as our pure game::screen_to_world.
            if (event.type == sf::Event::MouseButtonPressed) {
                const sf::Vector2f world =
                    window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}, game_view);
                click_marker.setPosition(world);
                has_marker = true;
            }
        }

        const float dt = clock.restart().asSeconds();

        // --- move the player (with tile collision, separate axes) -----------
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
        const float step = speed * dt;
        Vec2f try_x = player_pos;
        try_x.x += move.x * step;
        if (!box_hits_solid(map, try_x, player_size)) {
            player_pos = try_x;
        }
        Vec2f try_y = player_pos;
        try_y.y += move.y * step;
        if (!box_hits_solid(map, try_y, player_size)) {
            player_pos = try_y;
        }

        // --- follow + clamp the camera --------------------------------------
        const Vec2f player_center{player_pos.x + player_size.x * 0.5f,
                                  player_pos.y + player_size.y * 0.5f};
        const Vec2f cam = game::clamp_view_center(player_center, window_size, level_size);
        game_view.setCenter(cam.x, cam.y);

        // --- draw -----------------------------------------------------------
        window.clear();

        // 1) Parallax background, in SCREEN space (default view). Scrolling it
        //    is just sliding the sampled texture rectangle by the parallax
        //    offset; setRepeated(true) means it tiles to cover the window.
        window.setView(hud_view);
        const Vec2f off = game::parallax_offset(cam, parallax_factor);
        background.setTextureRect(sf::IntRect(static_cast<int>(off.x), static_cast<int>(off.y),
                                              static_cast<int>(win_w), static_cast<int>(win_h)));
        window.draw(background);

        // 2) The world, through the scrolling game view.
        window.setView(game_view);
        window.draw(tiles, tile_states);
        player.setPosition(player_pos.x, player_pos.y);
        window.draw(player);
        if (has_marker) {
            window.draw(click_marker);
        }

        // 3) The HUD, back in the fixed default view. The minimap dot uses a
        //    world->minimap-space conversion (player / level * panel size).
        window.setView(hud_view);
        window.draw(minimap);
        const Vec2f dot{minimap_origin.x + (player_center.x / level_size.x) * minimap_size.x,
                        minimap_origin.y + (player_center.y / level_size.y) * minimap_size.y};
        minimap_dot.setPosition(dot.x, dot.y);
        window.draw(minimap_dot);

        window.display();
    }

    return 0;
}
