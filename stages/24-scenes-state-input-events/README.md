# Stage 24 — Scenes, state machine, input & events

**Goal:** give the game *structure*. Up to now a mini-project has been one big loop. Real games are
organised into **scenes** (Menu, Playing, GameOver) managed on a **stack**, moved between by a
guarded **state machine**, fed by **mapped input** (semantic actions, not hardcoded keys), and glued
together by a small **event system** (publish/subscribe). You'll build all four as clean,
unit-tested subsystems and wire them into one demo.

**New concepts:** `enum class GameState` + a **state machine** with guarded transitions · the
canonical **`Scene`** interface and a **scene stack** (push/pop/top) · **input action mapping**
(keys → `Action`) · a **`std::function` event bus** (observer / pub-sub). (*Python comparisons:*
dict-based transition tables, an `abc.ABC` with abstract methods, a `{key: action}` dict, and
`defaultdict(list)` of callbacks.)

The `Scene` interface here is the **shared contract** stage 26 lifts into `namespace eng`, so its
shape (`update` / `render` / `handle_input` + virtual destructor) is fixed on purpose.

---

## 1. Scenes: one screen, one object

A game is rarely in one mode. The menu, the match, and the game-over screen each have their own
update logic, their own drawing, and their own input handling. Cramming all of that into one loop
with `if (state == MENU) ... else if (state == PLAYING) ...` grows unreadable fast.

The fix is a **`Scene`**: an abstract base class where each concrete screen is one object.

```cpp
// scene/scene.hpp
class Scene {
public:
    virtual ~Scene() = default;                             // delete-through-base is safe
    virtual void update(float dt) = 0;                      // advance dt seconds
    virtual void render(sf::RenderTarget& target) = 0;      // draw yourself
    virtual void handle_input(const sf::Event& event) = 0;  // react to one event
};
```

### Python comparison

This is a Python **abstract base class**:

```python
class Scene(abc.ABC):
    @abstractmethod
    def update(self, dt): ...
    @abstractmethod
    def render(self, target): ...
    @abstractmethod
    def handle_input(self, event): ...
```

`= 0` makes a method **pure virtual** — the C++ spelling of `@abstractmethod`. You can't
instantiate `Scene` directly; you subclass it (`MenuScene`, `PlayingScene`, …). The `virtual`
destructor is the stage-07 rule: because scenes are owned and deleted through a `Scene*`, the base
destructor must be `virtual` so the *derived* destructor actually runs.

> **A deliberate trick — forward declaration.** `scene.hpp` doesn't `#include` any SFML header; it
> just says `namespace sf { class RenderTarget; class Event; }`. A **forward declaration** ("this
> type exists; details elsewhere") is all you need to name a type in a *reference* parameter. The
> payoff: the `Scene` interface and the whole scene stack stay **SFML-free**, so their logic is
> unit-testable without a window. Only the concrete scenes (in the app) include real SFML to draw.

---

## 2. The scene stack

Why a *stack* and not a single `current_scene` variable? Because scenes nest. Pause a match and a
pause menu appears **on top of** the frozen game; dismiss it and the match is exactly as you left
it. That's a stack: push suspends what's below, pop resumes it.

```cpp
// scene/scene_stack.hpp  (SFML-free — see scene.hpp)
class SceneStack {
    std::vector<std::unique_ptr<Scene>> scenes_;   // owns its scenes (RAII, stage 11)
public:
    void push(std::unique_ptr<Scene> scene);
    void pop();
    Scene* top() const;            // active scene, or nullptr

    void update(float dt);                          // TOP scene only
    void handle_input(const sf::Event& event);      // TOP scene only
    void render(sf::RenderTarget& target);          // ALL scenes, bottom-to-top (overlays compose)
};
```

Ownership is via `std::unique_ptr` (stage 11): the stack is the single owner, and `pop()` destroys
the scene automatically. `update` and `handle_input` go to the **top scene only** (the one in
focus); `render` draws **every** scene bottom-to-top so an overlay layers over the game beneath it.

> **A subtle lifetime rule you'll meet in the mini-project.** A scene must **never pop itself**
> from inside its own `update`/`handle_input` — popping destroys the object while its method is
> still running (use-after-free). The demo solves this by having scenes *request* a transition
> (set a flag) that the main loop applies afterwards, at a safe point.

---

## 3. The state machine

Scenes are the *screens*; the **state machine** is the *rulebook* for moving between them. We model
the high-level state as a scoped enum and guard every transition:

```cpp
enum class GameState { Menu, Playing, GameOver };

// Only legal moves apply; illegal ones are rejected and leave the state unchanged.
Menu     --start-->  Playing
Playing  --lose --> GameOver     Playing --quit--> Menu
GameOver --menu --> Menu          GameOver --again--> Playing
```

`StateMachine::transition_to(to)` returns `true` only if the move is in the table, so gameplay code
can't shove the game from `Menu` straight to `GameOver`. Guarding transitions in one place makes
illegal states *unrepresentable* rather than a bug you hunt later.

### Python comparison

A state machine is a transition table plus a rejection rule — in Python usually a dict of dicts:

```python
LEGAL = {Menu: {Playing}, Playing: {GameOver, Menu}, GameOver: {Menu, Playing}}
def transition_to(self, to):
    if to not in LEGAL[self.state]: return False
    self.state = to; return True
```

Our C++ writes the table as a `switch` (see [`src/game_state.cpp`](project/src/game_state.cpp)),
but it's the same idea.

---

## 4. Input action mapping

Never scatter `if (key == W)` through gameplay code. The moment a player wants to rebind keys, or
you add arrow-key support, you're editing every file. Add one layer of indirection: raw keys map to
**semantic actions**, and gameplay only ever talks about actions.

```cpp
enum class Action { MoveUp, MoveDown, Confirm, Cancel, Quit };

input::InputMap map;
map.bind(static_cast<int>(sf::Keyboard::W),    Action::MoveUp);   // WASD...
map.bind(static_cast<int>(sf::Keyboard::Up),   Action::MoveUp);   // ...and arrows, same action
// gameplay: what does this key mean?  (returns std::optional<Action>)
if (map.action_for(code) == Action::MoveUp) { ... }
```

Rebinding is now a one-line change; supporting two keys for one action is two `bind` calls. Notice
`InputMap` stores plain **`int` key codes**, not `sf::Keyboard::Key` — that keeps it SFML-free and
unit-testable (the app passes `static_cast<int>(event.key.code)`). `action_for` returns
`std::optional<Action>` (stage 10): an unbound key is `std::nullopt`, C++'s `None`.

### Python comparison

A dict lookup with a default: `action = KEYMAP.get(code)` where a miss returns `None`.

---

## 5. The event bus (observer / pub-sub)

When a point is scored, several unrelated things must happen: the HUD updates, a sound plays, maybe
the state machine checks for a win. If the scoring code calls each of them directly, it becomes a
tangle of dependencies. The **observer pattern** inverts it: scoring just **emits** "point scored"
and forgets; interested parties **subscribe** ahead of time.

```cpp
events::EventBus bus;
bus.subscribe(EventType::PointScored, [](int who) { /* update HUD */ });
bus.subscribe(EventType::PointScored, [](int who) { /* play sound  */ });
bus.emit(EventType::PointScored, /*payload*/ 2);   // both handlers run; emitter doesn't care who
```

Handlers are stored as **`std::function<void(int)>`** (stage 09) — C++'s type-erased "any
callable", which can hold a free function or a capturing lambda. Emitting an event nobody listens
to is a harmless no-op.

### Python comparison

Exactly `defaultdict(list)` of callbacks: `handlers[event].append(cb)` then
`for cb in handlers[event]: cb(payload)`. Same as Qt signals/slots or JS `addEventListener`.

---

## 6. How it fits together (and what stays testable)

Four subsystems, three of them header-only, **all SFML-free**:

| subsystem | file | tested in |
|-----------|------|-----------|
| state machine | [`game/game_state.hpp`](project/include/game/game_state.hpp) | [`test_game_state.cpp`](project/tests/test_game_state.cpp) |
| input mapping | [`input/action.hpp`](project/include/input/action.hpp) | [`test_input_map.cpp`](project/tests/test_input_map.cpp) |
| event bus | [`events/event_bus.hpp`](project/include/events/event_bus.hpp) | [`test_event_bus.cpp`](project/tests/test_event_bus.cpp) |
| scene stack | [`scene/scene_stack.hpp`](project/include/scene/scene_stack.hpp) | [`test_scene_stack.cpp`](project/tests/test_scene_stack.cpp) |

Because the scene stack is SFML-free (forward-declared `sf` types), the test can push a **mock
scene** and verify push/pop/top and that only the top scene updates — no window required. The
concrete scenes' *rendering* stays unverified-by-execution; everything else is pinned by 12 tests.

---

## Building and running

The mini-project has its own [`project/CMakeLists.txt`](project/CMakeLists.txt). **The first
configure clones Catch2 (and, for the app, SFML 2.6.1) from GitHub, so it needs network access.**

```bash
cd project

# --- Fast path: tests only (no SFML, no display) ---
cmake -S . -B build -DBUILD_APP=OFF
cmake --build build
ctest --test-dir build --output-on-failure        # 12 tests — RED until you implement the TODOs

# --- Full path: also build the SFML demo ---
cmake -S . -B build
cmake --build build
./build/scenes_demo
```

In the demo: **Enter** starts the game, **W/S** or **Up/Down** move the paddle, **Enter** scores a
point (watch the event log on stderr), reaching 3 ends the round, **Esc** quits to the menu.

Exercises are a separate, SFML-free project:

```bash
cd exercises
cmake -S . -B build && cmake --build build
ctest --test-dir build --output-on-failure
# ...or by hand:  clang++ -std=c++17 -Wall -Wextra 04_event_bus.cpp -o ex && ./ex
```

> **Troubleshooting.** Link errors → add `-DCMAKE_CXX_COMPILER=/usr/bin/clang++`. macOS *"Could NOT
> find OpenGL"* during the SFML configure → pass `-DCMAKE_OSX_SYSROOT="$(xcrun --show-sdk-path)"`.

---

## Exercises

In [`exercises/`](exercises/). Starter `NN_name.cpp` + reference `NN_name.solution.cpp`, easy → hard.
Input mapping and the event system each get their own drill, as promised.

1. **`01_game_state`** *(easy)* — an `enum class GameState` and its cycle Menu→Playing→GameOver.
2. **`02_state_machine`** *(medium)* — implement guarded `transition_to`; reject illegal moves.
3. **`03_input_mapping`** *(medium)* — build the key→action map: `bind` and `action_for` (with
   `std::optional`), then rebind a key.
4. **`04_event_bus`** *(medium)* — implement `subscribe`/`emit` with `std::function`; multiple
   subscribers, event isolation.
5. **`05_scene_stack`** *(medium–hard)* — implement push/pop/top and "update the top only" with a
   mock scene.
6. **`06_pause_overlay`** *(hard)* — use the stack to push a pause overlay over a running game and
   pop it, proving the game freezes and then resumes intact.

---

## Your task — Menu / Playing / GameOver on a scene stack

**Your task.** [`project/`](project/) is a **starter**: the `Scene` interface, the concrete scenes
and window boilerplate in [`src/main.cpp`](project/src/main.cpp), and the tests are all given, but
the four subsystems' bodies are stubbed with `TODO`s. Implement them so the four tests go green:

- **State machine** — `is_legal` / `transition_to` in
  [`src/game_state.cpp`](project/src/game_state.cpp).
- **Input map** — `bind` / `action_for` / … in [`include/input/action.hpp`](project/include/input/action.hpp).
- **Event bus** — `subscribe` / `emit` / `subscriber_count` in
  [`include/events/event_bus.hpp`](project/include/events/event_bus.hpp).
- **Scene stack** — `push` / `pop` / `top` / `update` / … in
  [`src/scene_stack.cpp`](project/src/scene_stack.cpp).

The starter compiles and links, but `ctest` is **RED** until you fill in the `TODO`s — work through
the four `tests/test_*.cpp` files until green. When done it **combines all four concepts**: the
`MenuScene` / `PlayingScene` / `GameOverScene` each *request* a `GameState` transition; the main
loop asks the `StateMachine` whether it's legal and, if so, rebuilds the stack — safely, outside the
scene's own methods. Scenes read semantic *actions* (`Confirm`, `MoveUp`, `Cancel`), never raw keys,
and `emit` scoring / game-over events that subscribers in `main` react to without the scenes knowing
who listens.

Stuck? A complete reference is in [`project/solution/`](project/solution/) — build it standalone
(it has its own `CMakeLists.txt`) and compare:

```bash
cmake -S project/solution -B project/solution/build -DBUILD_APP=OFF
cmake --build project/solution/build
ctest --test-dir project/solution/build --output-on-failure   # all green
```

Text is drawn only if a system font loads (skipped gracefully otherwise), so the app builds and
links anywhere. Build + link of your version is verified; on-screen behaviour is
unverified-by-execution.

---

## Checklist before moving on

- [ ] I implemented the mini-project starter (state machine, input map, event bus, scene stack) so
      the four `tests/` suites are green.
- [ ] I can explain why games use a scene *stack* rather than a single current-scene variable.
- [ ] I know why the `Scene` base needs a `virtual` destructor, and why it forward-declares `sf`
      types instead of including SFML.
- [ ] My state machine rejects illegal transitions and leaves state unchanged.
- [ ] My input map decouples keys from actions, and I can rebind in one place.
- [ ] I built a `std::function` event bus and understand subscribe/emit as the observer pattern.
- [ ] I understand why a scene must not pop itself mid-method, and how deferring the transition
      fixes it.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md).

Next up: [Stage 25 — Resource manager & data-driven design](../25-resource-manager-data-driven/).
