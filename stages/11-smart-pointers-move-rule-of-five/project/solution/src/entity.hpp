// ============================================================================
// Stage 11 mini-project — Entities & a shared resource (header) — REFERENCE SOLUTION
// ============================================================================
//
// Demonstrates the two ownership models side by side:
//   - unique_ptr : each Character/Enemy has ONE owner (the entity vector).
//   - shared_ptr : one SpriteStub is SHARED by several entities; it lives until
//                  the last entity referencing it is gone (reference counting).
//
// SpriteStub stands in for a real texture/sprite (we're not on SFML yet). The
// point is the ownership graph, not the pixels.
//
// Character follows the RULE OF 0: it owns nothing raw — just a std::string, ints,
// and a std::shared_ptr, all of which manage themselves — so it declares NONE of
// the special members. (Contrast ScopedResource, which owns a raw handle and needs
// the full rule of 5.)
// ============================================================================
#pragma once

#include <memory>
#include <string>

// A pretend GPU resource. Loading is "expensive", so many entities share one.
struct SpriteStub {
    std::string texture;
    explicit SpriteStub(std::string t) : texture(std::move(t)) {}
};

class Character {
public:
    // The sprite is passed as a shared_ptr: constructing an entity bumps the
    // sprite's reference count; destroying the entity drops it.
    Character(std::string name, int hp, int attack, std::shared_ptr<SpriteStub> sprite)
        : name_(std::move(name)), hp_(hp), attack_(attack), sprite_(std::move(sprite)) {}

    // Virtual so the vector<unique_ptr<Character>> can delete derived types
    // correctly and dispatch describe()/attack() at runtime (stage 07).
    virtual ~Character() = default;

    virtual std::string describe() const {
        return name_ + " (hero, hp " + std::to_string(hp_) + ", atk " +
               std::to_string(attack_) + ", sprite '" + sprite_->texture + "')";
    }

    const std::string& name() const { return name_; }
    const SpriteStub& sprite() const { return *sprite_; }

protected:
    std::string name_;
    int hp_;
    int attack_;
    std::shared_ptr<SpriteStub> sprite_;  // shared ownership of the sprite
};

class Enemy : public Character {
public:
    Enemy(std::string name, int hp, int attack, std::shared_ptr<SpriteStub> sprite)
        : Character(std::move(name), hp, attack, std::move(sprite)) {}

    std::string describe() const override {
        return name_ + " (enemy, hp " + std::to_string(hp_) + ", atk " +
               std::to_string(attack_) + ", sprite '" + sprite_->texture + "')";
    }
};
