// ============================================================================
// Stage 27 capstone — ExploreScene (implementation) — YOUR TASK
// ============================================================================
//
// This is the STARTER. The ENGINE is provided complete and most of the scene's
// window/rendering WIRING is given for you (constructor setup, render, input
// dispatch, HUD text). YOUR job is the GLUE that connects the pure game logic to
// the engine — the three methods stubbed below:
//   - update()      : drive movement through game::resolve_move, collect items
//                     with game::try_pickup, and react with particles + sound.
//   - save_game()   : gather Stats + inventory + position and game::save() them.
//   - load_game()   : game::load() and restore Stats + inventory + position.
// (The pure functions those call live in world.*, inventory.*, save_data.* — the
// other half of your task, and what the unit tests cover.)
//
// The starter already compiles and links (each stubbed body has a placeholder), so
// `capstone` builds and opens a window; it just won't move/pickup/save until you
// fill the TODOs in.
//
// A complete reference is in ../solution/src/explore_scene.cpp — try it yourself first.
// ============================================================================
#include "explore_scene.hpp"

#include <string>

#include "eng/audio.hpp"
#include "eng/font.hpp"
#include "eng/textures.hpp"

namespace capstone {

namespace {

// A tiny fallback level, used only if assets/level.txt can't be found (e.g. the
// game is run from an unexpected directory). Guarantees the game still runs.
const char* kFallbackLevel =
    "10 8 32\n"
    "1 1 1 1 1 1 1 1 1 1\n"
    "1 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 1 1 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 1\n"
    "1 0 0 0 0 0 1 0 0 1\n"
    "1 0 0 1 0 0 0 0 0 1\n"
    "1 0 0 0 0 0 0 0 0 1\n"
    "1 1 1 1 1 1 1 1 1 1\n";

}  // namespace

// --- GIVEN: scene/window wiring. Sets up the world, camera, sprites, audio,
//     input and HUD from the provided engine. You do not need to change this.
ExploreScene::ExploreScene(const gmath::Vec2f& view_size, const std::string& asset_dir,
                           const std::string& save_path)
    : camera_(view_size), particles_(600), save_path_(save_path) {
    // --- World: load from a data file (data-driven design, stage 25), with an
    //     embedded fallback so the game is never un-runnable.
    if (!map_.load_from_file(asset_dir + "/level.txt")) {
        map_.load_from_string(kFallbackLevel);
    }
    // Tile id 1 is a wall (solid by default). Colour floor + wall; the renderer
    // skips any id with no colour, so this is also what "empty" would look like.
    tile_renderer_.set_color(0, sf::Color(40, 48, 58));    // floor
    tile_renderer_.set_color(1, sf::Color(92, 110, 140));  // wall
    tile_renderer_.build(map_);

    // Camera can't scroll past the map edges.
    camera_.set_bounds(gmath::AABB{{0.0f, 0.0f}, map_.pixel_size()});

    // --- Items scattered on floor tiles (positions chosen to sit on 0-tiles).
    items_ = {
        {"Health Potion", 15, {70.0f, 38.0f}, 20.0f, false},
        {"Iron Sword", 120, {646.0f, 102.0f}, 20.0f, false},
        {"Silver Key", 40, {390.0f, 390.0f}, 20.0f, false},
        {"Mana Potion", 20, {166.0f, 422.0f}, 20.0f, false},
        {"Gold Coin", 5, {550.0f, 326.0f}, 20.0f, false},
    };

    player_pos_ = gmath::Vec2f{48.0f, 48.0f};

    // --- Procedural sprites (no image files).
    player_texture_ = eng::make_solid_texture(static_cast<unsigned>(player_size_),
                                              static_cast<unsigned>(player_size_),
                                              sf::Color(250, 220, 80));
    item_texture_ = eng::make_circle_texture(20, sf::Color(120, 220, 255));
    player_sprite_.setTexture(player_texture_);
    item_sprite_.setTexture(item_texture_);

    // Pickup sparks fade from bright white to blue.
    particles_.set_color(eng::Rgba{220, 245, 255, 255}, eng::Rgba{70, 130, 255, 0});

    // --- Audio: a short synthesized "ding".
    pickup_buffer_ = eng::make_tone(680.0f, 0.12f);
    pickup_sound_.setBuffer(pickup_buffer_);

    // --- Input: name the movement actions once.
    input_.bind("up", sf::Keyboard::W);
    input_.bind("down", sf::Keyboard::S);
    input_.bind("left", sf::Keyboard::A);
    input_.bind("right", sf::Keyboard::D);

    // --- HUD text (only if a font was found).
    font_ok_ = eng::load_system_font(font_);
    if (font_ok_) {
        hud_text_.setFont(font_);
        hud_text_.setCharacterSize(16);
        hud_text_.setFillColor(sf::Color::White);
        hud_text_.setPosition(8.0f, 6.0f);
    }
    refresh_hud();
}

// GIVEN helper: the player's collider, derived from position + size.
gmath::AABB ExploreScene::player_box() const {
    return gmath::AABB{player_pos_, {player_size_, player_size_}};
}

void ExploreScene::update(float dt) {
    // TODO(stage 27): this is the per-frame conductor over the pure game logic.
    //   1. Build a movement intent from the two input axes:
    //      gmath::Vec2f dir{input_.axis("left","right"), input_.axis("up","down")};
    //      normalise it (so diagonals aren't faster) and scale by move_speed_ * dt.
    //   2. player_pos_ = game::resolve_move(map_, player_box(), delta);  // collision
    //   3. auto picked = game::try_pickup(items_, player_box(), inventory_); for each
    //      returned position, particles_.emit_burst(where, 28, 150.0f, 0.6f) and
    //      pickup_sound_.play(); refresh_hud() if anything was picked up.
    //   4. particles_.update(dt); camera_.follow(player_box().center());
    //   5. Tick down status_timer_; when it hits 0 clear status_ and refresh_hud().
    (void)dt;
}

// --- GIVEN: rendering. Draws the world through the camera, then the HUD in screen
//     space. You do not need to change this.
void ExploreScene::render(sf::RenderTarget& target) {
    // 1) World is drawn through the CAMERA view (scrolls with the player).
    camera_.apply(target);
    tile_renderer_.render(target);

    for (const game::WorldItem& item : items_) {
        if (item.collected) {
            continue;
        }
        item_sprite_.setPosition(item.pos.x, item.pos.y);
        target.draw(item_sprite_);
    }

    player_sprite_.setPosition(player_pos_.x, player_pos_.y);
    target.draw(player_sprite_);

    particles_.render(target);

    // 2) HUD is drawn in SCREEN space, so switch back to the default view. This
    //    is the world<->screen distinction from the camera stage: the HUD must
    //    not scroll with the world.
    target.setView(target.getDefaultView());
    if (font_ok_) {
        target.draw(hud_text_);
    }
}

// --- GIVEN: input dispatch. F5 saves, F9 loads (both delegate to your methods).
void ExploreScene::handle_input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::F5) {
            save_game();
        } else if (event.key.code == sf::Keyboard::F9) {
            load_game();
        }
    }
}

void ExploreScene::save_game() {
    // TODO(stage 27): pack the current Stats, inventory_ and player_pos_ into a
    // game::SaveData, call game::save(save_path_, data), and set status_ to
    // "Saved!" / "Save failed" accordingly. Then status_timer_ = 2.0f; refresh_hud().
}

void ExploreScene::load_game() {
    // TODO(stage 27): call game::load(save_path_). If it has_value(), restore
    // stats_, inventory_ and player_pos_ from it and set status_ = "Loaded!";
    // otherwise status_ = "No save found". Then status_timer_ = 2.0f; refresh_hud().
}

// --- GIVEN: builds the HUD string from the current stats + inventory + status.
void ExploreScene::refresh_hud() {
    if (!font_ok_) {
        return;
    }
    std::string text;
    text += "HP " + std::to_string(stats_.hp) + "   ATK " + std::to_string(stats_.attack) +
            "   DEF " + std::to_string(stats_.defense) + "\n";
    text += "Gold value: " + std::to_string(inventory_.total_value()) + "\n";
    text += "Bag (" + std::to_string(inventory_.distinct_count()) + "): ";
    for (const game::Item& item : inventory_.items()) {
        text += item.name + " x" + std::to_string(inventory_.count_of(item.name)) + "   ";
    }
    text += "\n[WASD] move    [F5] save    [F9] load";
    if (!status_.empty()) {
        text += "     " + status_;
    }
    hud_text_.setString(text);
}

}  // namespace capstone
