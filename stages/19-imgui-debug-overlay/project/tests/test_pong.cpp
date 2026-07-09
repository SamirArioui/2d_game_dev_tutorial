// ============================================================================
// Tests for the pure Pong logic.
//
// Every rule of the game — motion, wall bounce, paddle bounce + spin, scoring,
// serving — is a small function with a knowable answer, so we can pin all of it
// without ever opening a window. This is the stage's thesis in practice: the
// FUN of the game lives in code you can unit-test.
// ============================================================================

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "pong/pong.hpp"

using namespace pong;
using gmath::Vec2f;

TEST_CASE("integrate moves the ball by velocity * dt", "[pong][motion]") {
    Ball ball;
    ball.pos = Vec2f{100.0f, 100.0f};
    ball.vel = Vec2f{200.0f, -50.0f};
    integrate(ball, 0.5f);
    REQUIRE(ball.pos.x == Catch::Approx(200.0f));  // 100 + 200*0.5
    REQUIRE(ball.pos.y == Catch::Approx(75.0f));   // 100 + (-50)*0.5
}

TEST_CASE("clamp_paddle keeps a paddle inside the court", "[pong][paddle]") {
    const Court court;  // 800 x 600
    Paddle p;           // size 16 x 100

    SECTION("above the top snaps to 0") {
        p.pos = Vec2f{24.0f, -40.0f};
        clamp_paddle(p, court);
        REQUIRE(p.pos.y == Catch::Approx(0.0f));
    }
    SECTION("below the bottom snaps so the whole paddle stays visible") {
        p.pos = Vec2f{24.0f, 999.0f};
        clamp_paddle(p, court);
        REQUIRE(p.pos.y == Catch::Approx(court.height - p.size.y));  // 600 - 100
    }
}

TEST_CASE("bounce_off_walls reflects only at the top/bottom", "[pong][walls]") {
    const Court court;
    Ball ball;

    SECTION("no bounce while in bounds") {
        ball.pos = Vec2f{400.0f, 300.0f};
        ball.vel = Vec2f{0.0f, -100.0f};
        REQUIRE_FALSE(bounce_off_walls(ball, court));
    }
    SECTION("top wall pushes the ball back down") {
        ball.pos = Vec2f{400.0f, -5.0f};
        ball.vel = Vec2f{0.0f, -100.0f};
        REQUIRE(bounce_off_walls(ball, court));
        REQUIRE(ball.pos.y == Catch::Approx(0.0f));
        REQUIRE(ball.vel.y > 0.0f);  // now heading down
    }
    SECTION("bottom wall pushes the ball back up") {
        ball.pos = Vec2f{400.0f, 999.0f};
        ball.vel = Vec2f{0.0f, 100.0f};
        REQUIRE(bounce_off_walls(ball, court));
        REQUIRE(ball.pos.y == Catch::Approx(court.height - ball.size.y));
        REQUIRE(ball.vel.y < 0.0f);  // now heading up
    }
}

TEST_CASE("ball_hits_paddle detects overlap", "[pong][collision]") {
    Paddle p;
    p.pos = Vec2f{24.0f, 250.0f};  // spans x[24,40], y[250,350]
    Ball ball;

    SECTION("touching the paddle counts as a hit") {
        ball.pos = Vec2f{30.0f, 300.0f};  // overlaps the paddle
        REQUIRE(ball_hits_paddle(ball, p));
    }
    SECTION("far away is not a hit") {
        ball.pos = Vec2f{400.0f, 300.0f};
        REQUIRE_FALSE(ball_hits_paddle(ball, p));
    }
}

TEST_CASE("bounce_off_paddle reverses X, adds spin, keeps speed", "[pong][bounce]") {
    Paddle p;
    p.pos = Vec2f{24.0f, 250.0f};  // centre y = 300

    auto make_ball_at = [](float y) {
        Ball b;
        b.pos = Vec2f{30.0f, y};        // overlapping the paddle, centre x right of paddle
        b.vel = Vec2f{-400.0f, 0.0f};   // moving LEFT into the paddle
        return b;
    };

    SECTION("horizontal direction flips") {
        Ball b = make_ball_at(293.0f);  // dead centre
        bounce_off_paddle(b, p);
        REQUIRE(b.vel.x > 0.0f);  // was moving left, now moving right
    }
    SECTION("hitting above centre sends the ball up") {
        Ball b = make_ball_at(255.0f);  // above the paddle centre
        bounce_off_paddle(b, p);
        REQUIRE(b.vel.y < 0.0f);  // -y is up
    }
    SECTION("hitting below centre sends the ball down") {
        Ball b = make_ball_at(340.0f);  // below the paddle centre
        bounce_off_paddle(b, p);
        REQUIRE(b.vel.y > 0.0f);
    }
    SECTION("a centre hit stays roughly horizontal and keeps speed") {
        Ball b = make_ball_at(293.0f);
        bounce_off_paddle(b, p);
        REQUIRE(b.vel.y == Catch::Approx(0.0f).margin(1e-3));
        REQUIRE(gmath::length(b.vel) == Catch::Approx(400.0f));  // speed preserved
    }
    SECTION("the ball is nudged clear so it won't re-collide") {
        Ball b = make_ball_at(293.0f);
        bounce_off_paddle(b, p);
        REQUIRE_FALSE(ball_hits_paddle(b, p));
    }
}

TEST_CASE("check_scoring reports who scored", "[pong][score]") {
    const Court court;
    Ball ball;

    SECTION("ball off the LEFT edge -> right player scores") {
        ball.pos = Vec2f{-30.0f, 300.0f};
        REQUIRE(check_scoring(ball, court) == Scorer::Right);
    }
    SECTION("ball off the RIGHT edge -> left player scores") {
        ball.pos = Vec2f{820.0f, 300.0f};
        REQUIRE(check_scoring(ball, court) == Scorer::Left);
    }
    SECTION("ball in play -> nobody scores") {
        ball.pos = Vec2f{400.0f, 300.0f};
        REQUIRE(check_scoring(ball, court) == Scorer::None);
    }
}

TEST_CASE("serve centres the ball and launches it the right way", "[pong][serve]") {
    const Court court;

    SECTION("served to the left") {
        const Ball b = serve(court, /*to_left=*/true, 500.0f);
        REQUIRE(b.pos.x == Catch::Approx(court.width * 0.5f - b.size.x * 0.5f));
        REQUIRE(b.pos.y == Catch::Approx(court.height * 0.5f - b.size.y * 0.5f));
        REQUIRE(b.vel.x < 0.0f);
        REQUIRE(gmath::length(b.vel) == Catch::Approx(500.0f));
    }
    SECTION("served to the right") {
        const Ball b = serve(court, /*to_left=*/false, 500.0f);
        REQUIRE(b.vel.x > 0.0f);
    }
}

TEST_CASE("make_initial_state is a valid fresh game", "[pong][setup]") {
    const Court court;
    const GameState s = make_initial_state(court);

    REQUIRE(s.score_left == 0);
    REQUIRE(s.score_right == 0);
    // Both paddles fully inside the court vertically.
    REQUIRE(s.left.pos.y >= 0.0f);
    REQUIRE(s.left.pos.y + s.left.size.y <= court.height);
    REQUIRE(s.right.pos.y + s.right.size.y <= court.height);
    // Left paddle on the left half, right paddle on the right half.
    REQUIRE(s.left.pos.x < court.width * 0.5f);
    REQUIRE(s.right.pos.x > court.width * 0.5f);
}
