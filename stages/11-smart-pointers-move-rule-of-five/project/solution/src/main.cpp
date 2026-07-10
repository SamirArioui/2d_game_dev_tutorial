// ============================================================================
// Stage 11 mini-project — Entities, shared resources & the rule of 5 (demo) — REFERENCE SOLUTION
// ============================================================================
//
// Combines every concept of the stage:
//   - std::vector<std::unique_ptr<Character>>  : the vector exclusively owns each
//     entity; polymorphic describe() dispatches at runtime.
//   - std::shared_ptr<SpriteStub>              : one sprite SHARED by several
//     entities; use_count() shows ownership growing and shrinking.
//   - std::move                                : hand a named unique_ptr into the
//     vector (source left empty).
//   - ScopedResource                           : a rule-of-5 class whose special
//     members log, so copy/move/destroy become observable.
//
// This is the REFERENCE SOLUTION. The starter you implement lives two levels up
// in ../../src (fill in scoped_resource.cpp and this main.cpp). Build this
// finished version on its own:
//   cmake -S project/solution -B build-solution && cmake --build build-solution
//   ./build-solution/stage11_entities
// ============================================================================
#include "entity.hpp"
#include "scoped_resource.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

int main() {
    std::cout << "=== Shared sprite (shared_ptr) ===\n";
    // One sprite, shared by every goblin-family enemy below.
    auto goblin_sprite = std::make_shared<SpriteStub>("goblin.png");
    std::cout << "goblin.png owners: " << goblin_sprite.use_count() << " (just the loader)\n";

    std::cout << "\n=== Entities owned by a vector<unique_ptr<Character>> ===\n";
    std::vector<std::unique_ptr<Character>> entities;

    // The hero gets its own sprite (a temporary shared_ptr, moved straight in).
    entities.push_back(
        std::make_unique<Character>("Aria", 30, 12, std::make_shared<SpriteStub>("hero.png")));

    // Two enemies SHARE goblin_sprite: each copy of the shared_ptr bumps its count.
    entities.push_back(std::make_unique<Enemy>("Goblin", 12, 4, goblin_sprite));
    entities.push_back(std::make_unique<Enemy>("Orc", 20, 6, goblin_sprite));

    // Explicit std::move of a NAMED unique_ptr into the vector: ownership transfers,
    // and `slime` is left empty afterwards.
    auto slime = std::make_unique<Enemy>("Slime", 8, 2, goblin_sprite);
    std::cout << "before move: slime is " << (slime ? "owned" : "empty") << "\n";
    entities.push_back(std::move(slime));
    std::cout << "after  move: slime is " << (slime ? "owned" : "empty") << "\n";

    std::cout << "goblin.png owners now: " << goblin_sprite.use_count()
              << " (loader + Goblin + Orc + Slime)\n\n";

    // Runtime polymorphism through the base pointers.
    for (const auto& e : entities) {
        std::cout << "  " << e->describe() << "\n";
    }

    std::cout << "\n=== Releasing all entities ===\n";
    entities.clear();  // each unique_ptr frees its entity -> shared sprite count falls
    std::cout << "goblin.png owners after clear: " << goblin_sprite.use_count()
              << " (back to just the loader)\n";

    std::cout << "\n=== Rule of 5, observed (ScopedResource) ===\n";
    {
        ScopedResource a("sound", 100);   // ctor
        ScopedResource b = a;             // copy ctor (deep copy)
        ScopedResource c = std::move(a);  // move ctor (steal); a now empty
        std::cout << "  a is " << (a.empty() ? "empty (moved-from)" : "owning") << "\n";

        ScopedResource d("music", 200);   // ctor
        d = b;                            // copy assign
        d = std::move(c);                 // move assign; c now empty

        std::cout << "  leaving scope -> destructors run in reverse (d, c, b, a):\n";
    }  // a, b, c, d destroyed here

    return 0;
}
