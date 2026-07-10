// ============================================================================
// Stage 26 mini-project — PongScene (implementation) — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement the eng::Scene that turns the pure pong:: rules
// into a playable game, using the engine subsystems the header lists: eng::InputMap
// for named actions, eng::make_*_texture for procedural sprites, eng::make_tone for
// the beeps, and eng::load_system_font for the score text. The ENGINE is provided
// and complete — your job is to write this GAME layer on top of it.
//
// The class definition (members + method signatures) is in pong_scene.hpp. The
// starter already compiles and links (each body has a placeholder), so the `pong`
// executable builds — but it won't play correctly until you fill the TODOs in.
// (The pure rules in pong_logic.cpp are the other half of your task, and are what
// the unit tests cover.)
//
// A complete reference is in ../solution/src/pong_scene.cpp — try it yourself first.
// ============================================================================
#include "pong_scene.hpp"

#include <string>

#include "eng/audio.hpp"
#include "eng/font.hpp"
#include "eng/textures.hpp"

namespace pong {

PongScene::PongScene(const Config& config) : config_(config), state_(make_initial(config)) {
    // TODO(stage 26): set the scene up:
    //   - bind the four actions "left_up"/"left_down"/"right_up"/"right_down" to
    //     W/S and Up/Down with input_.bind(...).
    //   - build paddle_texture_ / ball_texture_ with eng::make_solid_texture /
    //     eng::make_circle_texture, then setTexture() on the three sprites.
    //   - synthesize bounce_buffer_ / score_buffer_ with eng::make_tone and point
    //     bounce_sound_ / score_sound_ at them (setBuffer).
    //   - font_ok_ = eng::load_system_font(font_); if found, style score_text_
    //     (size/colour) and call refresh_score_text().
}

void PongScene::refresh_score_text() {
    // TODO(stage 26): if font_ok_, set score_text_ to "<left>   :   <right>",
    // re-centre it horizontally (getLocalBounds + setOrigin) and position it at the
    // top-middle of the court.
}

void PongScene::update(float dt) {
    // TODO(stage 26): read the two paddle axes with input_.axis("left_up",
    // "left_down") / ("right_up","right_down"), advance the rules with
    // pong::step(state_, config_, dt, move_left, move_right), then react to the
    // returned Event: play bounce_sound_ on a wall/paddle bounce, or score_sound_ +
    // refresh_score_text() on a score.
    (void)dt;
}

void PongScene::render(sf::RenderTarget& target) {
    // TODO(stage 26): position the paddle/ball sprites from state_ + config_, draw
    // the dashed centre line, draw the three sprites, and (if font_ok_) the score.
    (void)target;
}

void PongScene::handle_input(const sf::Event& event) {
    // TODO(stage 26): on a KeyPressed of R, reset the match
    // (state_ = make_initial(config_)) and refresh_score_text(). Window close is
    // handled for you by eng::Application.
    (void)event;
}

}  // namespace pong
