# Stage 17 — Sprites, text & animation

**Goal:** put pictures and words on screen. You'll draw a **sprite** from a **texture**, animate it
by flipping through frames of a **sprite sheet**, drive those frames with a small **animation
controller**, and render a **text** label. Along the way you'll meet the pitfall that trips up
*everyone* new to SFML — the **texture-must-outlive-the-sprite** lifetime rule — and the
**working-directory / asset-path** trap that makes fonts and images "mysteriously" fail to load.

**New concepts:** `sf::Texture` vs `sf::Sprite` (data vs view) · texture lifetime & why a sprite
is basically a pointer · building textures in code with `sf::Image` · sprite sheets + frame math ·
an `enum class`-driven animation controller · `sf::Font` / `sf::Text` · loading assets robustly.

**Python comparison:** there is no close Python analogue to most of this (pygame hides texture
lifetimes from you). So the emphasis of this stage is a C++ idea you already started in stages 06
and 11: **resource lifetimes**. A sprite that points at a dead texture is the same bug as a
dangling pointer.

---

## 1. Texture vs Sprite — data and a view of it

Two objects, two jobs:

- **`sf::Texture`** is the *image data*, living in GPU memory. It is heavy: potentially megabytes
  of pixels. You load it once.
- **`sf::Sprite`** is a lightweight *positioned view* of a texture: a transform (position, scale,
  rotation) plus a rectangle saying *which part* of the texture to show. It is cheap to copy and
  move around every frame.

The key fact, and the whole reason this stage has a warning box: **a sprite does not own or copy
its texture. It stores a pointer to one.** So:

> **The texture must outlive every sprite that uses it.**

This is [stage 06](../06-pointers-references-asan/)'s dangling-pointer lesson wearing a game-dev
costume. If the texture is destroyed while a sprite still refers to it, drawing that sprite reads
freed memory — you get a white rectangle, garbage pixels, or a crash.

```cpp
// THE BUG (exercise 03): a sprite bound to a texture that dies immediately.
sf::Sprite make_sprite() {
    sf::Texture texture;           // local: destroyed when the function returns
    /* ...load into texture... */
    return sf::Sprite(texture);    // returned sprite now points at freed memory
}                                  // <-- texture destroyed here; sprite dangles
```

```cpp
// THE FIX: the caller owns the texture, so it lives long enough.
sf::Texture texture;                       // declared first -> destroyed last
sf::Sprite sprite = make_sprite(texture);  // pass the long-lived texture in
```

In [`project/src/main.cpp`](project/src/main.cpp) both `texture` and `sprite` are locals in
`main`, with `texture` declared **first**. C++ destroys locals in reverse order, so the sprite is
always destroyed before the texture it points at — **RAII** (stage 07/11) enforcing the rule for
free. When you later store sprites and textures in a class, keep them in the same object so their
lifetimes are tied together (that's what the resource manager in stage 25 will formalize).

---

## 2. Building a texture in code with `sf::Image`

This repo ships **no binary art files**. Instead we synthesize images at runtime:

- **`sf::Image`** is a CPU-side pixel buffer. `image.create(w, h, color)` allocates it;
  `image.setPixel(x, y, color)` writes one pixel.
- **`sf::Texture`** is the GPU-side copy. `texture.loadFromImage(image)` uploads it.

The mini-project's `make_walk_sheet()` paints a little figure into each 32×32 cell of a
5-frame sheet. In a real project you'd instead call `texture.loadFromFile("player.png")` — but the
lifetime rules and everything below are identical.

> `loadFromImage` / `loadFromFile` return a **`bool`**. Check it. A failed load leaves the texture
> empty and you'll draw nothing — always test the result and report the failure (stage 10's
> error-handling habit).

---

## 3. Sprite sheets & frame math

A **sprite sheet** is one image holding many frames in a grid. To show frame *N* you set the
sprite's **texture rect** — the rectangle of the texture to display:

```cpp
sprite.setTextureRect(sf::IntRect(left, top, width, height));
```

Everything about *which* rectangle is pure integer arithmetic, so we keep it out of the render
code entirely. In [`project/include/anim/animation.hpp`](project/include/anim/animation.hpp):

```cpp
IntRect frame_to_rect(int frame, int columns, int frame_w, int frame_h);
// column = frame % columns;  row = frame / columns;
// {column*frame_w, row*frame_h, frame_w, frame_h}
```

`IntRect` here is our own SFML-free struct; `main.cpp` copies it into an `sf::IntRect` at the last
moment. That separation is the recurring theme: **logic stays pure and testable, SFML lives only
at the edges.**

---

## 4. Time → frame: the animation controller

Animation is "advance a timer, pick a frame". A **clip** is a run of frames played at some `fps`:

```cpp
struct Clip { int first_frame; int frame_count; float fps; bool loop; };
```

The pure function that converts elapsed time into a frame index:

```cpp
int frame_index(const Clip& clip, float elapsed) {
    int frames_advanced = int(elapsed * clip.fps);            // whole frames played
    if (clip.loop) return frames_advanced % clip.frame_count; // wrap around
    return std::min(frames_advanced, clip.frame_count - 1);   // else hold last
}
```

The **animation controller** (`anim::Animator`) is the small state machine on top: it holds one
`Clip` per `enum class State` (`Idle`, `Walk`), tracks the current state and a timer, and exposes
`current_sheet_frame()`. The one rule worth burning in:

> `set_state()` resets the timer **only when the state actually changes.** If you naively reset it
> every call, holding the walk key re-selects `Walk` 60×/second and the character freezes on frame
> 0. This is a real bug people ship — and it's why we unit-test the controller.

All of section 3 and 4 is covered by [`tests/test_animation.cpp`](project/tests/test_animation.cpp)
and runs headless under `ctest`.

---

## 5. Text: `sf::Font` + `sf::Text`, and the asset-path trap

Text needs a font, and a font is a `.ttf` file loaded at runtime:

```cpp
sf::Font font;
if (!font.loadFromFile("assets/DejaVuSans.ttf")) { /* handle it! */ }
sf::Text text;
text.setFont(font);            // sf::Text points at the font — SAME lifetime rule as textures!
text.setString("Score: 0");
text.setCharacterSize(24);
text.setFillColor(sf::Color::White);
```

Two pitfalls:

1. **Font lifetime.** `sf::Text` stores a pointer to its `sf::Font`, exactly like sprite→texture.
   The font must outlive the text.
2. **The working-directory trap.** `loadFromFile("assets/font.ttf")` is resolved relative to the
   program's **current working directory**, *not* the location of the executable. Run the same
   binary from a different folder and the load fails. This is the single most common "it works on
   my machine" asset bug. For the course we sidestep it by trying **absolute** system font paths
   and **failing gracefully** (a warning, then run without the label) rather than crashing. You
   supply a font: a stock macOS install has `/System/Library/Fonts/Supplemental/Arial.ttf`; most
   Linux distros have `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf`.

---

## Building and running

**Toolchain:** CMake ≥ 3.16 and a C++17 compiler. The first configure **git-clones SFML 2.6.1**
(and Catch2) via `FetchContent`, so it needs network the first time.

The mini-project in [`project/`](project/) ships as a **starter**: the SFML boilerplate and the
sprite-sheet/text setup are complete, but the `anim` frame-math bodies in
[`project/src/animation.cpp`](project/src/animation.cpp) are left as `TODO`s, so out of the box
`ctest` is **RED**. You implement them to make it green.

```bash
cd project
# The first configure clones SFML 2.6.1 + Catch2. If your default compiler's
# runtime doesn't link cleanly, pin Apple/LLVM clang as shown.
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build

# Run the pure logic tests (headless-friendly — this is what CI runs):
ctest --test-dir build --output-on-failure    # RED until you implement animation.cpp

# Run the animated app (needs a display / GPU):
./build/walk_cycle            # arrow keys walk; a label shows state + frame
```

> **Tip:** to iterate on just the tested logic without compiling SFML at all:
> `cmake --build build --target anim_tests && ctest --test-dir build`.

The complete, all-green reference is in [`project/solution/`](project/solution/) — build it the
same way to see `100% tests passed`:

```bash
cmake -S project/solution -B project/solution/build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build project/solution/build
ctest --test-dir project/solution/build --output-on-failure
```

**On this course's headless build machine**, the app is verified to **compile and link** against
SFML; its on-screen behavior is *unverified-by-execution*. The frame math it relies on is fully
covered by unit tests that **do** run here.

---

## Exercises

In [`exercises/`](exercises/). Pure drills (`01`, `02`, `06`) compile with a bare
`clang++ -std=c++17 file.cpp` and print `...passed`; SFML drills (`03`, `04`, `05`) build through
the folder's `CMakeLists.txt` and need a display to *see*. Ordered easy → hard.

1. **`01_frame_at`** *(easy, pure)* — turn elapsed time into a frame index (loop vs one-shot).
2. **`02_sheet_rects`** *(easy, pure)* — map a frame index to its rectangle on the grid.
3. **`03_texture_lifetime`** *(medium, SFML)* — find and fix a **dangling-texture** bug. The
   stage's key pitfall, hands-on.
4. **`04_procedural_texture`** *(medium, SFML)* — paint an `sf::Image` checkerboard and upload it
   to a texture — no art file needed.
5. **`05_text_label`** *(medium, SFML)* — load a font with graceful fallback and draw `sf::Text`.
6. **`06_animator`** *(medium–hard, pure)* — build the little state-machine controller and prove
   that re-selecting the same state does **not** rewind the timer.

Build the exercise folder:

```bash
cd exercises
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build
./build/01_frame_at_solution          # pure drills print "...passed"
./build/03_texture_lifetime_solution  # SFML drills open a window
```

---

## Mini-project — animated walk-cycle sprite with a label

**Your task.** In [`project/`](project/). The SFML shell is done for you — the procedurally-built
sprite sheet, the texture/sprite lifetime setup, the `sf::Text` label, and the game loop in
[`src/main.cpp`](project/src/main.cpp) are all complete. What's left is the **frame math**: the
`anim::Animator` and free-function bodies in
[`src/animation.cpp`](project/src/animation.cpp) are stubbed with `// TODO(stage 17)` placeholders,
so the starter compiles and links but [`tests/test_animation.cpp`](project/tests/test_animation.cpp)
starts **RED**. Implement `frame_index`, `frame_to_rect`, and the `Animator` methods (`set_state`,
`update`, `current_frame_in_clip`, `current_sheet_frame`) until `ctest` is green.

The finished project **combines every concept of the stage**:

- a 5-frame sprite sheet **built procedurally** with `sf::Image` (idle pose + 4 walk frames),
- an `sf::Texture` that **outlives** its `sf::Sprite` (RAII, both locals in `main`),
- the SFML-free **`anim::Animator`** choosing the frame each tick,
- `frame_to_rect` picking the texture rect,
- an `sf::Text` label (loaded with graceful fallback) showing the live state and frame.

Arrow keys walk the character (mirrored to face its direction). The logic split — pure `anim`
library + thin SFML `main` — is exactly the pattern the rest of the course scales up: the ball and
paddles in **Pong** (stage 18) will keep their physics in a tested library and render at the edges
the same way.

Stuck? The complete reference is in [`project/solution/`](project/solution/) — build it with
`cmake -S project/solution -B project/solution/build -DCMAKE_CXX_COMPILER=/usr/bin/clang++` and
compare, but try it yourself first.

---

## Checklist before moving on

- [ ] I can explain why a sprite is "basically a pointer to a texture", and what breaks if the
      texture dies first.
- [ ] I built a texture from an `sf::Image` in code and checked the `bool` result.
- [ ] I can compute a frame's texture rect from a sheet index, and a frame index from elapsed time.
- [ ] My animation controller only rewinds the timer on a *real* state change.
- [ ] I loaded a font, handled the missing-file case without crashing, and know the
      working-directory trap.
- [ ] `ctest` runs the animation tests green.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — the SFML tutorials on *Graphics →
Sprites and textures* and *Graphics → Text and fonts*.

Next up: [Stage 18 — First real game: Pong](../18-first-game-pong/).
