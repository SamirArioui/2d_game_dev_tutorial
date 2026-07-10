// ============================================================================
// Stage 11 mini-project — Entities, shared resources & the rule of 5 (demo) — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement main() below (and fill in scoped_resource.cpp) to
// combine every concept of the stage:
//   - std::vector<std::unique_ptr<Character>>  : the vector exclusively owns each
//     entity; polymorphic describe() dispatches at runtime.
//   - std::shared_ptr<SpriteStub>              : one sprite SHARED by several
//     entities; use_count() shows ownership growing and shrinking.
//   - std::move                                : hand a named unique_ptr into the
//     vector (source left empty).
//   - ScopedResource                           : a rule-of-5 class whose special
//     members log, so copy/move/destroy become observable.
//
// The Character / Enemy / SpriteStub types are declared in entity.hpp and the
// ScopedResource contract in scoped_resource.hpp. The starter already compiles,
// links and runs (main just returns 0) — produce the README's output by filling in
// the TODOs here and in scoped_resource.cpp.
//
// A complete reference is in ../solution/src/main.cpp — try it yourself first.
//
// Build & run (from this stage folder):
//   cmake -S . -B build && cmake --build build
//   ./build/stage11_entities
// ============================================================================
#include "entity.hpp"
#include "scoped_resource.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

int main() {
    // TODO(stage 11): make a shared sprite with std::make_shared<SpriteStub>("goblin.png")
    // and print its use_count() (should be 1 — just the loader).

    // TODO(stage 11): create a std::vector<std::unique_ptr<Character>>. push_back a
    // hero Character (with its own std::make_shared<SpriteStub>("hero.png")) and two
    // Enemy entities (Goblin, Orc) that SHARE the goblin sprite.

    // TODO(stage 11): make a NAMED unique_ptr (a Slime), print whether it is owned,
    // std::move it into the vector, then print that it is now empty.

    // TODO(stage 11): print the shared sprite's use_count() again (loader + 3 goblins
    // = 4), then loop the vector calling e->describe() for runtime polymorphism.

    // TODO(stage 11): entities.clear() and print use_count() (back to 1) to show the
    // shared_ptr reference count falling as owners are released.

    // TODO(stage 11): in a nested scope, exercise ScopedResource so its rule-of-5
    // logging is visible: construct one, copy it, std::move it, copy-assign and
    // move-assign, then let the scope end so the destructors run in reverse.

    return 0;  // placeholder — the starter runs but prints nothing until implemented
}
