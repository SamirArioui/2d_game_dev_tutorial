#pragma once

// (REFERENCE SOLUTION — the starter version is two levels up in ../../src/)
// ============================================================================
// PongScene — Pong rebuilt as an eng::Scene on top of libengine.
//
// Compare with stage 18, where Pong WAS the program: it owned main(), the
// window and the loop. Here Pong is just a Scene. It has no loop of its own —
// eng::Application drives it. That is the proof the engine is reusable: the same
// Application that will run the stage-27 capstone runs this, unchanged.
//
// Engine subsystems this scene uses:
//   - Scene / Application  : lifecycle + the loop (inversion of control)
//   - InputMap             : "move_up"/"move_down" actions, not raw keys
//   - textures             : procedural paddle/ball sprites (no image files)
//   - audio                : synthesized beep on each bounce/score
//   - collision + math     : via the pure pong:: rules
// ============================================================================

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "eng/input.hpp"
#include "eng/scene.hpp"
#include "pong_logic.hpp"

namespace pong {

class PongScene : public eng::Scene {
public:
    explicit PongScene(const Config& config);

    void update(float dt) override;
    void render(sf::RenderTarget& target) override;
    void handle_input(const sf::Event& event) override;

private:
    void refresh_score_text();

    Config config_;
    State state_;
    eng::InputMap input_;

    // Procedural sprites (their textures are members so they outlive the sprites).
    sf::Texture paddle_texture_;
    sf::Texture ball_texture_;
    sf::Sprite left_paddle_;
    sf::Sprite right_paddle_;
    sf::Sprite ball_;

    // Text/HUD. font_ok_ records whether a usable font was found so we degrade
    // gracefully (no crash, just no score text) on a machine with none.
    sf::Font font_;
    bool font_ok_{false};
    sf::Text score_text_;

    // Audio. Buffers must outlive the sounds that point at them -> members.
    sf::SoundBuffer bounce_buffer_;
    sf::SoundBuffer score_buffer_;
    sf::Sound bounce_sound_;
    sf::Sound score_sound_;
};

}  // namespace pong
