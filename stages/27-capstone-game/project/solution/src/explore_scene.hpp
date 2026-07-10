#pragma once

// (REFERENCE SOLUTION — the starter version is two levels up in ../../src/)
// ============================================================================
// ExploreScene — the capstone: a top-down explorer built on libengine.
//
// This one Scene wires together nearly every engine subsystem:
//   Tilemap + TileRenderer  the world you walk on / around
//   Camera                  follows the player, clamped to the map edges
//   collision (resolve_move) solid-tile collision (pure logic in world.*)
//   Inventory + try_pickup  items on the map go into a bag on pickup (pure logic)
//   ParticleSystem          a burst of sparks at each pickup
//   audio                   a pickup "ding" (synthesized, no files)
//   font + sf::Text         the HUD (stats, gold, item counts, controls)
//   SaveData save/load      F5 saves position+inventory+stats; F9 loads
//
// The gameplay RULES (movement collision, pickup, save format) live in the pure,
// unit-tested files world.* / save_data.* / inventory.*. This scene is the thin
// layer that draws them and feeds them input.
// ============================================================================

#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "eng/camera.hpp"
#include "eng/input.hpp"
#include "eng/particles.hpp"
#include "eng/scene.hpp"
#include "eng/tile_renderer.hpp"
#include "eng/tilemap.hpp"
#include "inventory.hpp"
#include "save_data.hpp"
#include "world.hpp"

namespace capstone {

class ExploreScene : public eng::Scene {
public:
    // view_size is the visible world area (usually the window size). asset_dir is
    // where level.txt lives; save_path is where F5/F9 read and write.
    ExploreScene(const gmath::Vec2f& view_size, const std::string& asset_dir,
                 const std::string& save_path);

    void update(float dt) override;
    void render(sf::RenderTarget& target) override;
    void handle_input(const sf::Event& event) override;

private:
    gmath::AABB player_box() const;
    void refresh_hud();
    void save_game();
    void load_game();

    // World data + rendering.
    eng::Tilemap map_;
    eng::TileRenderer tile_renderer_;
    eng::Camera camera_;
    eng::ParticleSystem particles_;

    // Game state.
    game::Stats stats_{30, 12, 8};
    game::Inventory inventory_;
    std::vector<game::WorldItem> items_;
    gmath::Vec2f player_pos_;
    float player_size_{24.0f};
    float move_speed_{170.0f};
    std::string save_path_;
    std::string status_;         // transient "Saved!" / "Loaded!" message
    float status_timer_{0.0f};   // seconds the status message stays on screen

    // Input.
    eng::InputMap input_;

    // Rendering resources (textures kept as members so sprites stay valid).
    sf::Texture player_texture_;
    sf::Texture item_texture_;
    sf::Sprite player_sprite_;
    sf::Sprite item_sprite_;

    // HUD.
    sf::Font font_;
    bool font_ok_{false};
    sf::Text hud_text_;

    // Audio.
    sf::SoundBuffer pickup_buffer_;
    sf::Sound pickup_sound_;
};

}  // namespace capstone
