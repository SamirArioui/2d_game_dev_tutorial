// ============================================================================
// Stage 19 mini-project — Pong with a Dear ImGui debug overlay.
//
// Same Pong as stage 18 (rules still in the tested pong_core library), plus an
// immediate-mode debug panel drawn on top with imgui-sfml. The panel exposes:
//   * a live FPS readout (rolling average via debug::FrameProfiler),
//   * a "Ball speed" slider that changes the ball's speed LIVE (debug::set_ball_speed),
//   * an entity counter,
//   * a pause checkbox.
//
// IMMEDIATE-MODE UI (the big idea): there is no widget tree to build and keep in
// sync. Every frame you literally re-issue the whole panel — "there is a slider
// here bound to this float" — and ImGui figures out the rest. Contrast with
// RETAINED-mode toolkits (tkinter/Qt) where you construct widget objects once
// and mutate them via callbacks. See the README for the full comparison.
//
// The five-call lifecycle: Init -> (per frame: ProcessEvent, Update, ...build UI...,
// Render) -> Shutdown.
//
// VERIFICATION: built headless, this file is checked to COMPILE + LINK against
// SFML + Dear ImGui + imgui-sfml. The overlay/window are not run here; the logic
// behind the widgets (set_ball_speed, FrameProfiler) is covered by ctest.
// ============================================================================

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>
#include <string>

#include "debug/tunables.hpp"
#include "gmath/sfml_bridge.hpp"
#include "pong/pong.hpp"

namespace {

bool load_a_font(sf::Font& font) {
    const std::array<const char*, 3> candidates{
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:/Windows/Fonts/arial.ttf",
    };
    for (const char* path : candidates) {
        if (font.loadFromFile(path)) {
            return true;
        }
    }
    std::cerr << "WARNING: no font found; the scoreboard will not render.\n";
    return false;
}

sf::RectangleShape shape_for(const gmath::Vec2f& pos, const gmath::Vec2f& size, sf::Color color) {
    sf::RectangleShape shape(gmath::to_sfml(size));
    shape.setPosition(gmath::to_sfml(pos));
    shape.setFillColor(color);
    return shape;
}

}  // namespace

int main() {
    const pong::Court court;
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned>(court.width),
                                         static_cast<unsigned>(court.height)),
                            "Stage 19 - Pong + ImGui");
    window.setFramerateLimit(60);

    // 1) INIT ImGui-SFML once, after the window exists.
    if (!ImGui::SFML::Init(window)) {
        std::cerr << "ImGui::SFML::Init failed\n";
        return 1;
    }

    pong::GameState state = pong::make_initial_state(court, /*serve_to_left=*/true);

    sf::Font font;
    const bool have_font = load_a_font(font);
    sf::Text score_text;
    if (have_font) {
        score_text.setFont(font);
        score_text.setCharacterSize(48);
        score_text.setFillColor(sf::Color(180, 180, 180));
        score_text.setPosition(court.width * 0.5f - 60.0f, 20.0f);
    }

    debug::FrameProfiler profiler(120);  // average FPS over ~2 seconds
    float ball_speed = pong::DefaultBallSpeed;  // the slider's backing value
    bool paused = false;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // 2) Feed every event to ImGui FIRST so it can claim clicks/keys on
            // the panel before the game reacts to them.
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
        if (dt > 0.05f) {
            dt = 0.05f;  // same tunnelling guard as stage 18
        }
        profiler.add_sample(dt);

        // 3) UPDATE ImGui (it needs the frame's dt as an sf::Time).
        ImGui::SFML::Update(window, elapsed);

        // --- build the debug panel (immediate mode: rebuilt every frame) ----
        ImGui::Begin("Debug");
        ImGui::Text("FPS: %.1f", static_cast<double>(profiler.fps()));
        ImGui::Text("Frame: %.2f ms", static_cast<double>(profiler.average_dt() * 1000.0f));
        // SliderFloat returns true on the frames the user drags it. Apply the
        // change LIVE by rescaling the ball's velocity (direction preserved).
        if (ImGui::SliderFloat("Ball speed", &ball_speed, 100.0f, 900.0f)) {
            debug::set_ball_speed(state.ball, ball_speed);
        }
        const int entity_count = 3;  // 2 paddles + 1 ball
        ImGui::Text("Entities: %d", entity_count);
        ImGui::Checkbox("Pause", &paused);
        if (ImGui::Button("Reset match")) {
            state = pong::make_initial_state(court, true);
            debug::set_ball_speed(state.ball, ball_speed);
        }
        ImGui::End();

        // --- simulate (unless paused) ---------------------------------------
        if (!paused) {
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

            pong::integrate(state.ball, dt);
            pong::bounce_off_walls(state.ball, court);
            if (pong::ball_hits_paddle(state.ball, state.left)) {
                pong::bounce_off_paddle(state.ball, state.left);
            } else if (pong::ball_hits_paddle(state.ball, state.right)) {
                pong::bounce_off_paddle(state.ball, state.right);
            }

            const pong::Scorer scorer = pong::check_scoring(state.ball, court);
            if (scorer != pong::Scorer::None) {
                if (scorer == pong::Scorer::Left) {
                    state.score_left += 1;
                    state.ball = pong::serve(court, false, ball_speed);
                } else {
                    state.score_right += 1;
                    state.ball = pong::serve(court, true, ball_speed);
                }
                // Keep the slider's speed after a re-serve.
                debug::set_ball_speed(state.ball, ball_speed);
            }
        }

        if (have_font) {
            score_text.setString(std::to_string(state.score_left) + "   " +
                                 std::to_string(state.score_right));
        }

        // --- render: game first, then the ImGui overlay on top --------------
        window.clear(sf::Color(15, 15, 20));
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

        // 4) RENDER ImGui after the game so the panel sits on top.
        ImGui::SFML::Render(window);
        window.display();
    }

    // 5) SHUTDOWN ImGui-SFML.
    ImGui::SFML::Shutdown();
    return 0;
}
