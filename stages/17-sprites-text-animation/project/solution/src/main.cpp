// ============================================================================
// Stage 17 mini-project — an animated walk-cycle sprite with a text label.
//
// It combines every concept of the stage:
//   * sf::Image  -> we BUILD a sprite sheet procedurally in code (no .png to
//                   ship), so this repo carries no binary assets.
//   * sf::Texture / sf::Sprite  -> and the lifetime rule that a texture MUST
//                   outlive every sprite that uses it (see the big note below).
//   * the SFML-free Animator (anim/animation.hpp) drives which frame shows.
//   * sf::Font / sf::Text -> an on-screen label, loaded with GRACEFUL fallback
//                   so a missing font never hard-crashes the program.
//
// NOTE ON VERIFICATION: this course is built headless, so this file is checked
// to COMPILE and LINK against SFML, but its on-screen behavior is not run here.
// The frame math it relies on IS covered by unit tests (tests/test_animation).
// ============================================================================

#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>

#include "anim/animation.hpp"
#include "gmath/sfml_bridge.hpp"
#include "gmath/vec2.hpp"

namespace {

constexpr int FrameSize = 32;  // each frame is 32x32 px on the sheet
constexpr int FrameCount = 5;  // frame 0 = idle pose, frames 1..4 = walk cycle
constexpr int Columns = FrameCount;

// ---------------------------------------------------------------------------
// Build the sprite sheet in memory. Real games load a .png here; we synthesize
// one so the tutorial needs no binary files. Each 32x32 cell draws a tiny
// figure; the four walk frames shift the "legs" so the motion is visible.
// ---------------------------------------------------------------------------
sf::Image make_walk_sheet() {
    sf::Image sheet;
    // create(width, height, fillColor) — transparent background.
    sheet.create(FrameSize * FrameCount, FrameSize, sf::Color::Transparent);

    // Leg x-offsets per frame; index 0 (idle) keeps the legs together.
    const std::array<int, FrameCount> leg_shift{0, -4, 0, 4, 0};

    for (int f = 0; f < FrameCount; ++f) {
        const int ox = f * FrameSize;  // left edge of this frame on the sheet
        const sf::Color body = (f == 0) ? sf::Color(120, 170, 255)  // idle = blue
                                         : sf::Color(255, 200, 90);  // walk = amber

        // Head (a 8x8 block near the top).
        for (int y = 4; y < 12; ++y) {
            for (int x = 12; x < 20; ++x) {
                sheet.setPixel(ox + x, y, body);
            }
        }
        // Torso (a 10-wide column down the middle).
        for (int y = 12; y < 22; ++y) {
            for (int x = 11; x < 21; ++x) {
                sheet.setPixel(ox + x, y, body);
            }
        }
        // Two legs, shifted per frame so the walk reads as motion.
        const int shift = leg_shift[f];
        for (int y = 22; y < 30; ++y) {
            for (int x = 12; x < 15; ++x) {
                sheet.setPixel(ox + x + shift, y, body);       // left leg
                sheet.setPixel(ox + x + 5 - shift, y, body);   // right leg
            }
        }
    }
    return sheet;
}

// Try a few well-known font locations and load the first that works. Returns
// true on success. We NEVER crash on a missing font — we just report it and let
// the caller decide (here: skip drawing the label).
bool load_a_font(sf::Font& font) {
    // Documented default paths. On your machine, drop any .ttf here or edit
    // this list. The macOS system Arial exists on a stock install.
    const std::array<const char*, 3> candidates{
        "/System/Library/Fonts/Supplemental/Arial.ttf",        // macOS
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",     // Debian/Ubuntu
        "C:/Windows/Fonts/arial.ttf",                          // Windows
    };
    for (const char* path : candidates) {
        if (font.loadFromFile(path)) {
            std::cout << "Loaded font: " << path << '\n';
            return true;
        }
    }
    std::cerr << "WARNING: no font found. Put a .ttf at one of the documented "
                 "paths to see the label. Continuing without text.\n";
    return false;
}

}  // namespace

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Stage 17 - Walk cycle");
    window.setFramerateLimit(60);

    // ------------------------------------------------------------------------
    // THE TEXTURE LIFETIME RULE (the headline pitfall of this stage).
    //
    // sf::Sprite does NOT own or copy its texture — it stores a *pointer* to
    // one. So the texture must live at least as long as every sprite using it.
    // Here `texture` and `sprite` are both locals in main(); `texture` is
    // declared FIRST, so it is destroyed LAST — the sprite can never outlive
    // it. That is RAII working for us (stage 07/11).
    //
    // The classic bug (see exercises/03_texture_lifetime): build the texture as
    // a LOCAL inside a helper and return the sprite. The texture dies at the
    // end of the helper, the returned sprite points at freed memory, and you
    // get a white rectangle or a crash. Tie this back to dangling pointers
    // from stage 06: a sprite is essentially a pointer to a texture.
    // ------------------------------------------------------------------------
    sf::Texture texture;
    if (!texture.loadFromImage(make_walk_sheet())) {
        std::cerr << "Failed to build the sprite sheet texture.\n";
        return 1;
    }
    texture.setSmooth(false);  // crisp pixels when we scale up

    sf::Sprite sprite(texture);
    sprite.setScale(6.0f, 6.0f);                  // 32px -> 192px on screen
    sprite.setOrigin(FrameSize / 2.0f, FrameSize / 2.0f);
    gmath::Vec2f position{320.0f, 260.0f};        // our own vector for the logic
    sprite.setPosition(gmath::to_sfml(position)); // bridged to SFML at the edge

    // The animation controller: idle is the single frame 0; walk is frames 1..4
    // at 10 fps, looping. All the "which frame now?" logic is tested elsewhere.
    anim::Animator animator(anim::Clip{0, 1, 1.0f, true},
                            anim::Clip{1, 4, 10.0f, true});

    // Text label (optional if no font is available).
    sf::Font font;
    const bool have_font = load_a_font(font);
    sf::Text label;
    if (have_font) {
        label.setFont(font);
        label.setCharacterSize(20);
        label.setFillColor(sf::Color::White);
        label.setPosition(16.0f, 16.0f);
    }

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const float dt = clock.restart().asSeconds();

        // --- input: real-time key state (not events) drives movement ---------
        const bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        const bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        const bool moving = left || right;

        // Drive the state machine. set_state() only rewinds on a real change,
        // so holding a key keeps the cycle running smoothly.
        animator.set_state(moving ? anim::State::Walk : anim::State::Idle);
        animator.update(dt);

        // Move using our Vec2f math (reuse of the motion idea from stage 15).
        const float speed = 160.0f;  // px/sec
        if (left) {
            position.x -= speed * dt;
            sprite.setScale(-6.0f, 6.0f);  // face left by mirroring on X
        }
        if (right) {
            position.x += speed * dt;
            sprite.setScale(6.0f, 6.0f);
        }
        sprite.setPosition(gmath::to_sfml(position));

        // --- pick the texture rect for the current frame --------------------
        const anim::IntRect r =
            anim::frame_to_rect(animator.current_sheet_frame(), Columns, FrameSize, FrameSize);
        sprite.setTextureRect(sf::IntRect(r.left, r.top, r.width, r.height));

        if (have_font) {
            label.setString(std::string("Arrow keys to walk | state=") +
                            (moving ? "Walk" : "Idle") +
                            " frame=" + std::to_string(animator.current_frame_in_clip()));
        }

        // --- render ----------------------------------------------------------
        window.clear(sf::Color(30, 30, 40));
        window.draw(sprite);
        if (have_font) {
            window.draw(label);
        }
        window.display();
    }
    return 0;
}
