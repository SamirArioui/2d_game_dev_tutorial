// (REFERENCE SOLUTION — the starter version is two levels up in ../../src/)
#include "pong_scene.hpp"

#include <string>

#include "eng/audio.hpp"
#include "eng/font.hpp"
#include "eng/textures.hpp"

namespace pong {

PongScene::PongScene(const Config& config) : config_(config), state_(make_initial(config)) {
    // --- Input: name the ACTIONS, bind keys once. Left player W/S, right Up/Down.
    input_.bind("left_up", sf::Keyboard::W);
    input_.bind("left_down", sf::Keyboard::S);
    input_.bind("right_up", sf::Keyboard::Up);
    input_.bind("right_down", sf::Keyboard::Down);

    // --- Sprites from procedurally generated textures (no image files).
    paddle_texture_ = eng::make_solid_texture(static_cast<unsigned>(config_.paddle_w),
                                              static_cast<unsigned>(config_.paddle_h),
                                              sf::Color::White);
    ball_texture_ = eng::make_circle_texture(static_cast<unsigned>(config_.ball_size),
                                             sf::Color(240, 240, 120));
    left_paddle_.setTexture(paddle_texture_);
    right_paddle_.setTexture(paddle_texture_);
    ball_.setTexture(ball_texture_);

    // --- Audio: two synthesized beeps. Buffers are members; sounds point at them.
    bounce_buffer_ = eng::make_tone(520.0f, 0.06f);
    score_buffer_ = eng::make_tone(180.0f, 0.25f);
    bounce_sound_.setBuffer(bounce_buffer_);
    score_sound_.setBuffer(score_buffer_);

    // --- Text: only if a font was found; otherwise we render without a score.
    font_ok_ = eng::load_system_font(font_);
    if (font_ok_) {
        score_text_.setFont(font_);
        score_text_.setCharacterSize(48);
        score_text_.setFillColor(sf::Color::White);
        refresh_score_text();
    }
}

void PongScene::refresh_score_text() {
    if (!font_ok_) {
        return;
    }
    score_text_.setString(std::to_string(state_.left_score) + "   :   " +
                          std::to_string(state_.right_score));
    // Re-centre horizontally now that the string width may have changed.
    const sf::FloatRect bounds = score_text_.getLocalBounds();
    score_text_.setOrigin(bounds.left + bounds.width * 0.5f, 0.0f);
    score_text_.setPosition(config_.court_w * 0.5f, 12.0f);
}

void PongScene::update(float dt) {
    // Turn the two input axes into paddle motion, then advance the pure rules.
    const float move_left = input_.axis("left_up", "left_down");
    const float move_right = input_.axis("right_up", "right_down");

    const Event event = step(state_, config_, dt, move_left, move_right);

    // React to what happened — the ONLY place gameplay touches the outside world.
    switch (event) {
        case Event::WallBounce:
        case Event::PaddleBounce:
            bounce_sound_.play();
            break;
        case Event::ScoreLeft:
        case Event::ScoreRight:
            score_sound_.play();
            refresh_score_text();
            break;
        case Event::None:
            break;
    }
}

void PongScene::render(sf::RenderTarget& target) {
    left_paddle_.setPosition(config_.paddle_margin, state_.left_paddle_y);
    right_paddle_.setPosition(config_.court_w - config_.paddle_margin - config_.paddle_w,
                              state_.right_paddle_y);
    ball_.setPosition(state_.ball_pos.x, state_.ball_pos.y);

    // A dashed centre line, drawn as a few faint rectangles.
    sf::RectangleShape dash(sf::Vector2f(4.0f, 18.0f));
    dash.setFillColor(sf::Color(80, 80, 100));
    for (float y = 0.0f; y < config_.court_h; y += 34.0f) {
        dash.setPosition(config_.court_w * 0.5f - 2.0f, y);
        target.draw(dash);
    }

    target.draw(left_paddle_);
    target.draw(right_paddle_);
    target.draw(ball_);
    if (font_ok_) {
        target.draw(score_text_);
    }
}

void PongScene::handle_input(const sf::Event& event) {
    // Press R to reset the match. (Quitting is handled by the window's close
    // button, which eng::Application turns into shutdown.)
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
        state_ = make_initial(config_);
        refresh_score_text();
    }
}

}  // namespace pong
