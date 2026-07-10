// ============================================================================
// Stage 18 mini-project — Pong.
//
// This is the payoff: a full, playable game. Notice how SMALL this file is.
// All the rules live in the tested `pong` library (pong/pong.hpp); main() just:
//   1. reads the keyboard,
//   2. calls the pure step functions,
//   3. draws rectangles + a score,
//   4. beeps on a bounce.
//
// Controls:  left paddle = W / S,  right paddle = Up / Down.
//
// AUDIO PITFALL (mirrors stage 17's texture rule): sf::Sound stores a POINTER
// to its sf::SoundBuffer — it does NOT copy the samples. So the buffer MUST
// outlive the sound. We declare `beep_buffer` before `beep_sound` (destroyed
// after it) exactly like texture-before-sprite. Get this wrong and you hear
// silence or garbage, or crash on shutdown.
//
// VERIFICATION: built headless, this is checked to COMPILE + LINK against SFML
// (graphics/window/system/audio). Window/audio behavior is not run here; the
// game LOGIC is fully covered by ctest (tests/test_pong, tests/test_collision).
// ============================================================================

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "gmath/sfml_bridge.hpp"
#include "pong/pong.hpp"

namespace {

// Build a short "beep" entirely from samples — no .wav file needed. We fill a
// buffer with a sine wave and fade it out so it doesn't click at the end.
sf::SoundBuffer make_beep(float frequency_hz, float seconds) {
    const unsigned sample_rate = 44100;
    const unsigned count = static_cast<unsigned>(seconds * sample_rate);
    std::vector<sf::Int16> samples(count);

    constexpr float kTwoPi = 6.28318530718f;
    for (unsigned i = 0; i < count; ++i) {
        const float t = static_cast<float>(i) / sample_rate;
        const float fade = 1.0f - static_cast<float>(i) / count;  // linear fade-out
        const float value = std::sin(kTwoPi * frequency_hz * t) * 0.30f * fade;
        samples[i] = static_cast<sf::Int16>(value * 32767.0f);
    }

    sf::SoundBuffer buffer;
    // loadFromSamples(samples, count, channelCount, sampleRate): 1 = mono.
    if (!buffer.loadFromSamples(samples.data(), count, 1, sample_rate)) {
        std::cerr << "WARNING: failed to build beep buffer; running muted.\n";
    }
    return buffer;
}

bool load_a_font(sf::Font& font) {
    const std::array<const char*, 3> candidates{
        "/System/Library/Fonts/Supplemental/Arial.ttf",     // macOS
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",  // Linux
        "C:/Windows/Fonts/arial.ttf",                       // Windows
    };
    for (const char* path : candidates) {
        if (font.loadFromFile(path)) {
            return true;
        }
    }
    std::cerr << "WARNING: no font found; the scoreboard will not render.\n";
    return false;
}

// Convert a pong entity's box into an SFML rectangle we can draw.
sf::RectangleShape shape_for(const gmath::Vec2f& pos, const gmath::Vec2f& size, sf::Color color) {
    sf::RectangleShape shape(gmath::to_sfml(size));
    shape.setPosition(gmath::to_sfml(pos));
    shape.setFillColor(color);
    return shape;
}

}  // namespace

int main() {
    const pong::Court court;  // 800 x 600
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned>(court.width),
                                         static_cast<unsigned>(court.height)),
                            "Stage 18 - Pong");
    window.setFramerateLimit(60);

    pong::GameState state = pong::make_initial_state(court, /*serve_to_left=*/true);

    // --- audio: buffer FIRST, sound SECOND (lifetime rule) ------------------
    sf::SoundBuffer beep_buffer = make_beep(440.0f, 0.06f);  // a short A4 blip
    sf::Sound beep_sound;
    beep_sound.setBuffer(beep_buffer);  // stores a pointer into beep_buffer

    // --- scoreboard ---------------------------------------------------------
    sf::Font font;
    const bool have_font = load_a_font(font);
    sf::Text score_text;
    if (have_font) {
        score_text.setFont(font);
        score_text.setCharacterSize(48);
        score_text.setFillColor(sf::Color(180, 180, 180));
        score_text.setPosition(court.width * 0.5f - 60.0f, 20.0f);
    }

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();
        // Guard against a huge dt after a stall (e.g. the window was dragged):
        // a giant step could tunnel the ball straight through a paddle.
        if (dt > 0.05f) {
            dt = 0.05f;
        }

        // --- input: real-time key state moves the paddles -------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            state.left.pos.y -= state.left.speed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            state.left.pos.y += state.left.speed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            state.right.pos.y -= state.right.speed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            state.right.pos.y += state.right.speed * dt;
        }
        pong::clamp_paddle(state.left, court);
        pong::clamp_paddle(state.right, court);

        // --- simulate the ball ----------------------------------------------
        pong::integrate(state.ball, dt);

        if (pong::bounce_off_walls(state.ball, court)) {
            beep_sound.play();
        }
        if (pong::ball_hits_paddle(state.ball, state.left)) {
            pong::bounce_off_paddle(state.ball, state.left);
            beep_sound.play();
        } else if (pong::ball_hits_paddle(state.ball, state.right)) {
            pong::bounce_off_paddle(state.ball, state.right);
            beep_sound.play();
        }

        // --- scoring: whoever's edge the ball crossed loses; re-serve -------
        const pong::Scorer scorer = pong::check_scoring(state.ball, court);
        if (scorer == pong::Scorer::Left) {
            state.score_left += 1;
            state.ball = pong::serve(court, /*to_left=*/false, pong::DefaultBallSpeed);
        } else if (scorer == pong::Scorer::Right) {
            state.score_right += 1;
            state.ball = pong::serve(court, /*to_left=*/true, pong::DefaultBallSpeed);
        }

        if (have_font) {
            score_text.setString(std::to_string(state.score_left) + "   " +
                                 std::to_string(state.score_right));
        }

        // --- render ---------------------------------------------------------
        window.clear(sf::Color(15, 15, 20));

        // dashed centre line
        for (float y = 0.0f; y < court.height; y += 30.0f) {
            window.draw(shape_for(gmath::Vec2f{court.width * 0.5f - 2.0f, y + 6.0f},
                                  gmath::Vec2f{4.0f, 18.0f}, sf::Color(60, 60, 70)));
        }

        window.draw(shape_for(state.left.pos, state.left.size, sf::Color::White));
        window.draw(shape_for(state.right.pos, state.right.size, sf::Color::White));
        window.draw(shape_for(state.ball.pos, state.ball.size, sf::Color(120, 230, 140)));
        if (have_font) {
            window.draw(score_text);
        }

        window.display();
    }
    return 0;
}
