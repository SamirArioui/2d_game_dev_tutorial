// ============================================================================
// Stage 24 mini-project — Menu / Playing / GameOver on a scene stack, driven by mapped input and
// wired together with an event bus.
//
// How the four ideas fit together here:
//   - GameState + StateMachine : the legal high-level flow (Menu -> Playing -> GameOver -> Menu).
//   - SceneStack               : the concrete screen for each state, pushed/popped by the loop.
//   - InputMap                 : raw SFML keys -> semantic Actions, so scenes never hardcode keys.
//   - EventBus                 : scenes announce "point scored" / "game ended" without knowing who
//                                is listening (here: a HUD/log subscriber set up in main).
//
// SAFE TRANSITIONS: a scene never pops itself (that would destroy `this` mid-method). Instead it
// sets ctx.requested_state, and the main loop performs the transition afterwards, at a safe point.
//
// Only this file touches SFML. It is verified to BUILD and LINK; with no display in CI its
// on-screen behaviour is unverified-by-execution.
//
// Build & run:
//   cmake -S . -B build && cmake --build build
//   ./build/scenes_demo
// ============================================================================

#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>

#include "events/event_bus.hpp"
#include "game/game_state.hpp"
#include "input/action.hpp"
#include "scene/scene.hpp"
#include "scene/scene_stack.hpp"

namespace {

constexpr unsigned WindowWidth = 640;
constexpr unsigned WindowHeight = 480;
constexpr float PaddleStep = 24.0f;  // how far one MoveUp/MoveDown nudge moves the paddle
constexpr int WinScore = 3;

// Everything a scene needs to talk to the rest of the game. Passed by reference to each scene.
struct GameContext {
    sf::RenderWindow& window;
    events::EventBus& bus;
    input::InputMap& input;
    const sf::Font* font;                       // nullptr if no font could be loaded
    std::optional<game::GameState> requested;   // a scene sets this to REQUEST a transition
    int winner{0};
};

// Small helper: build an sf::Text only if we actually have a font (SFML needs a font to lay out
// glyphs). Returns an empty, harmless Text otherwise.
sf::Text make_text(const GameContext& ctx, const std::string& str, unsigned size, float x, float y) {
    sf::Text text;
    if (ctx.font != nullptr) {
        text.setFont(*ctx.font);
        text.setString(str);
        text.setCharacterSize(size);
        text.setPosition(x, y);
    }
    return text;
}

// --- Menu -------------------------------------------------------------------------------------
class MenuScene : public scene::Scene {
public:
    explicit MenuScene(GameContext& ctx) : ctx_(ctx) {}

    void update(float /*dt*/) override {}

    void handle_input(const sf::Event& event) override {
        if (event.type != sf::Event::KeyPressed) {
            return;
        }
        // The scene speaks in ACTIONS, never in raw keys.
        const auto action = ctx_.input.action_for(static_cast<int>(event.key.code));
        if (action == input::Action::Confirm) {
            ctx_.requested = game::GameState::Playing;  // ask to start the game
        } else if (action == input::Action::Quit) {
            ctx_.window.close();
        }
    }

    void render(sf::RenderTarget& target) override {
        target.clear(sf::Color(30, 30, 60));
        auto title = make_text(ctx_, "PONG-ish", 48, 210.0f, 120.0f);
        auto hint = make_text(ctx_, "ENTER: play    ESC: quit", 20, 160.0f, 260.0f);
        target.draw(title);
        target.draw(hint);
    }

private:
    GameContext& ctx_;
};

// --- Playing ----------------------------------------------------------------------------------
class PlayingScene : public scene::Scene {
public:
    explicit PlayingScene(GameContext& ctx)
        : ctx_(ctx), paddle_y_(WindowHeight / 2.0f - PaddleHeight / 2.0f) {}

    void update(float dt) override {
        // A ball bouncing off the top and bottom edges — just enough motion to see the scene live.
        ball_x_ += ball_vx_ * dt;
        ball_y_ += ball_vy_ * dt;
        if (ball_y_ < 0.0f || ball_y_ + BallSize > WindowHeight) {
            ball_vy_ = -ball_vy_;
        }
        if (ball_x_ < 0.0f || ball_x_ + BallSize > WindowWidth) {
            ball_vx_ = -ball_vx_;
        }
    }

    void handle_input(const sf::Event& event) override {
        if (event.type != sf::Event::KeyPressed) {
            return;
        }
        const auto action = ctx_.input.action_for(static_cast<int>(event.key.code));
        if (action == input::Action::MoveUp) {
            paddle_y_ -= PaddleStep;
        } else if (action == input::Action::MoveDown) {
            paddle_y_ += PaddleStep;
        } else if (action == input::Action::Confirm) {
            // Pretend the player scored. ANNOUNCE it; whoever cares (the HUD subscriber) reacts.
            ++score_;
            ctx_.bus.emit(events::EventType::PointScored, score_);
            if (score_ >= WinScore) {
                ctx_.winner = 1;
                ctx_.bus.emit(events::EventType::GameEnded, score_);
                ctx_.requested = game::GameState::GameOver;
            }
        } else if (action == input::Action::Cancel) {
            ctx_.requested = game::GameState::Menu;  // quit back to the menu
        }
    }

    void render(sf::RenderTarget& target) override {
        target.clear(sf::Color(10, 40, 10));

        sf::RectangleShape paddle({PaddleWidth, PaddleHeight});
        paddle.setPosition(20.0f, paddle_y_);
        paddle.setFillColor(sf::Color::White);
        target.draw(paddle);

        sf::RectangleShape ball({BallSize, BallSize});
        ball.setPosition(ball_x_, ball_y_);
        ball.setFillColor(sf::Color(255, 220, 80));
        target.draw(ball);

        target.draw(make_text(ctx_, "score: " + std::to_string(score_), 20, 20.0f, 20.0f));
    }

private:
    static constexpr float PaddleWidth = 16.0f;
    static constexpr float PaddleHeight = 96.0f;
    static constexpr float BallSize = 14.0f;

    GameContext& ctx_;
    float paddle_y_;
    float ball_x_{WindowWidth / 2.0f};
    float ball_y_{WindowHeight / 2.0f};
    float ball_vx_{220.0f};
    float ball_vy_{160.0f};
    int score_{0};
};

// --- GameOver ---------------------------------------------------------------------------------
class GameOverScene : public scene::Scene {
public:
    explicit GameOverScene(GameContext& ctx) : ctx_(ctx) {}

    void update(float /*dt*/) override {}

    void handle_input(const sf::Event& event) override {
        if (event.type != sf::Event::KeyPressed) {
            return;
        }
        const auto action = ctx_.input.action_for(static_cast<int>(event.key.code));
        if (action == input::Action::Confirm) {
            ctx_.requested = game::GameState::Menu;  // back to the menu
        }
    }

    void render(sf::RenderTarget& target) override {
        target.clear(sf::Color(60, 20, 20));
        target.draw(make_text(ctx_, "GAME OVER", 48, 190.0f, 140.0f));
        target.draw(make_text(ctx_, "ENTER: menu", 20, 230.0f, 260.0f));
    }

private:
    GameContext& ctx_;
};

// Build the scene for a state. "Full-screen" states REPLACE the stack (an overlay would instead
// push without clearing — see exercise 06).
std::unique_ptr<scene::Scene> make_scene(GameContext& ctx, game::GameState state) {
    switch (state) {
        case game::GameState::Menu:
            return std::make_unique<MenuScene>(ctx);
        case game::GameState::Playing:
            return std::make_unique<PlayingScene>(ctx);
        case game::GameState::GameOver:
            return std::make_unique<GameOverScene>(ctx);
    }
    return nullptr;
}

void enter_state(GameContext& ctx, scene::SceneStack& stack, game::GameState state) {
    while (!stack.empty()) {
        stack.pop();
    }
    stack.push(make_scene(ctx, state));
}

}  // namespace

int main() {
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Stage 24 — Scenes & state");
    window.setFramerateLimit(60);

    // Try a few common system font paths; text is skipped gracefully if none load (build/CI-safe).
    sf::Font font;
    const bool has_font = font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf") ||
                          font.loadFromFile("/Library/Fonts/Arial.ttf") ||
                          font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

    // The four subsystems, all owned by main and shared with scenes through the context.
    events::EventBus bus;
    input::InputMap input;
    game::StateMachine machine;

    // Bind physical keys to semantic actions in ONE place. Rebinding later touches only this block.
    input.bind(static_cast<int>(sf::Keyboard::W), input::Action::MoveUp);
    input.bind(static_cast<int>(sf::Keyboard::S), input::Action::MoveDown);
    input.bind(static_cast<int>(sf::Keyboard::Up), input::Action::MoveUp);
    input.bind(static_cast<int>(sf::Keyboard::Down), input::Action::MoveDown);
    input.bind(static_cast<int>(sf::Keyboard::Enter), input::Action::Confirm);
    input.bind(static_cast<int>(sf::Keyboard::Space), input::Action::Confirm);
    input.bind(static_cast<int>(sf::Keyboard::Escape), input::Action::Cancel);

    // Observers: react to gameplay events without the gameplay code knowing about us.
    bus.subscribe(events::EventType::PointScored,
                  [](int score) { sf::err() << "[event] point scored, score now " << score << "\n"; });
    bus.subscribe(events::EventType::GameEnded,
                  [](int score) { sf::err() << "[event] game ended at score " << score << "\n"; });

    GameContext ctx{window, bus, input, has_font ? &font : nullptr, std::nullopt, 0};

    scene::SceneStack stack;
    enter_state(ctx, stack, machine.state());  // boot into the Menu (the machine's start state)

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                stack.handle_input(event);  // the top scene turns keys into actions
            }
        }

        const float dt = clock.restart().asSeconds();
        stack.update(dt);

        // Apply any transition a scene requested — SAFELY, outside the scene's own methods.
        if (ctx.requested.has_value()) {
            if (machine.transition_to(*ctx.requested)) {
                enter_state(ctx, stack, machine.state());
            }
            ctx.requested.reset();
        }

        window.clear();
        stack.render(window);
        window.display();
    }

    return 0;
}
