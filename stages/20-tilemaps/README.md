# Stage 20 — Tilemaps

**Goal:** build the data structure behind every 2D level made of square tiles — a grid of integer
tile-ids — and learn to (a) **load it from a text file**, (b) **render it efficiently** with a
single `sf::VertexArray` instead of thousands of sprites, and (c) do **tile-based collision** so a
character can walk the level and bump into walls. The map logic is a **pure, unit-tested** class;
the rendering is a thin SFML layer on top — the same "logic-vs-rendering" split the course has
insisted on since stage 13.

**New concepts:** the tile-grid data structure (a 2D grid stored as one flat `std::vector`) ·
row-major indexing (`y * width + x`) · loading a map from a text file · rendering a whole map in
**one draw call** with `sf::VertexArray` + a procedural tileset texture · tile-based collision via
world→cell conversion · unit-testing the parser (dimensions, `tile_at`, malformed input).

**Python comparison:** a tilemap is a **2D list of ints** — `grid[y][x]` — but typed, fixed-shape,
and stored flat.

---

## 1. What a tilemap *is* (you already know this from Python)

A tile level is just a grid of small integers. In Python you'd reach for a list of lists:

```python
grid = [
    [1, 1, 1, 1],   # row 0  — a solid wall
    [1, 0, 0, 1],   # row 1  — walls on the sides, open in the middle
    [1, 1, 1, 1],   # row 2
]
tile = grid[y][x]   # the id at column x, row y
```

Each number is a **tile id**: `0` means empty/walkable, and anything non-zero is a solid tile
(different non-zero ids can *look* different — grass, stone, dirt — but for collision the rule is
just "non-zero = solid"). That's the entire idea. A real level is bigger and loaded from a file, but
it's the same 2D grid of ints.

### The three C++ differences

| | Python 2D list | C++ `Tilemap` |
|---|---|---|
| element type | anything | every cell is an `int` (compiler-checked) |
| shape | ragged rows allowed | fixed `W × H`, decided at load |
| storage | list of lists | **one** flat `std::vector<int>`, indexed `y*width + x` |

**Why store it flat?** A `vector<vector<int>>` scatters each row somewhere different in memory. One
contiguous `vector<int>` of length `W*H` is a single cache-friendly block — and it's exactly the
order the renderer and the GPU want to walk. The cost is that *you* do the index arithmetic:
`tiles_[y * width_ + x]`. (Exercise 01 drills precisely this.)

---

## 2. The `Tilemap` class — pure data, no SFML

Look at [`project/include/game/tilemap.hpp`](project/include/game/tilemap.hpp) and
[`project/src/tilemap.cpp`](project/src/tilemap.cpp). The class holds `width_`, `height_`,
`tile_size_` and the flat `tiles_` vector, and exposes exactly the course-wide contract:

```cpp
int  tile_at(int x, int y) const;      // id at a cell; 0 (empty) if off the map
bool solid_at(gmath::Vec2f world) const; // is the tile under this WORLD point solid?
```

Two design decisions worth calling out, both about making the class pleasant to *use* and to
*test*:

- **Out-of-range reads return `0`, they don't throw.** Collision code constantly asks about cells
  near the edge; "off the map reads as empty" means it never has to bounds-check first.
- **`solid_at` uses `std::floor`, not an `int` cast.** Casting truncates toward zero, so
  `-0.5 → 0` (wrong — that point is in the cell *left* of the origin). `floor(-0.5) → -1` is
  correct. Small detail, real bug if you skip it.

Crucially, **`tilemap.hpp` includes no SFML.** It uses `gmath::Vec2f` (a pure math type) and
nothing else. That is what lets the parser be unit-tested with no window (section 6).

### On-disk text format (the course-wide contract)

```
W H tileSize        <- first line: three ints
id id id ... id      <- then H rows, each of W space-separated ids
```

Parsing (`from_text`) reads the header with `>>`, then reads exactly `W*H` ints. `>>` skips all
whitespace, so it doesn't care how rows wrap — it only checks the **count**. Any of: missing/bad
header, non-positive dimension, a non-integer token, too few or too many tiles → it throws
`std::runtime_error`. `from_file` just opens the file (RAII `ifstream`, from stage 10), slurps it,
and hands the text to the same tested `from_text`.

---

## 3. Rendering: one `sf::VertexArray`, not N sprites

The naive way to draw a 20×15 map is 300 `sf::Sprite`s, each its own `window.draw()` call. That's
300 draw calls **per frame** — the GPU hates it. The right way, and the standard SFML idiom, is a
single **`sf::VertexArray`**: a list of vertices you fill yourself and draw in **one** call.

A vertex has a **position** (where on screen), a **texCoords** (where in a texture to sample), and a
colour. With `sf::Quads`, every 4 consecutive vertices form one textured rectangle. So the tilemap
becomes: *for each non-empty tile, append 4 vertices* — its 4 world corners, with texture
coordinates pointing at that id's cell in a **tileset** image.

[`project/src/tilemap_render.cpp`](project/src/tilemap_render.cpp) does two things:

- `make_tileset_texture(...)` — builds the tileset **procedurally** with `sf::Image`
  (`setPixel` per pixel), so there are **no binary asset files** in the repo. It's a horizontal
  strip of coloured cells (id 1 = grass-green, 2 = stone-grey, 3 = dirt-brown), each with a darker
  border so the grid reads clearly.
- `build_tilemap_vertices(...)` — walks the grid and appends 4 vertices per solid tile, wiring each
  quad's `texCoords` to its id's cell. Empty tiles (id 0) are simply skipped — they cost nothing.

The texture must **outlive** the vertex array that samples it, so `main` keeps both alive together
(a lifetime rule you met with textures/sprites in stage 17).

> **Further reading:** for *huge* maps you only build vertices for the tiles the camera can see
> ("culling") — that's a stage 21 exercise. Spatial partitioning (quadtrees/grids) for very large
> worlds is noted as out-of-scope in the roadmap.

---

## 4. Tile-based collision — slide along walls

The character is an axis-aligned box. To move it we use the **separate-axes** trick, the standard
approach for tile collision:

1. Try to move on **X** only. If the box's new position overlaps a solid tile, cancel the X move.
2. Then try to move on **Y** only. Cancel if it hits.

Doing the axes independently is what lets you **slide** along a wall instead of stopping dead when
you push diagonally into it. The overlap test (`box_hits_solid` in
[`project/src/main.cpp`](project/src/main.cpp)) just asks `map.solid_at(corner)` for the box's four
corners — because the character is smaller than a tile, the corners are enough. (Exercise 04 builds
this corner check; the mini-project uses it.)

---

## 5. Building and running

The mini-project lives in [`project/`](project/) with three CMake targets: the pure `tilemap_core`
library, the SFML `tilemap_demo` app, and the Catch2 `tilemap_tests` runner. It ships as a
**starter**: the SFML renderer, the window, and the headers are complete, but the pure `Tilemap`
methods in [`project/src/tilemap.cpp`](project/src/tilemap.cpp) are left for you to write, so out of
the box the parser tests are **RED**. You make them green.

```bash
cd project
cmake -S . -B build          # first run clones SFML 2.6.1 + Catch2 v3.7.1 (needs network)
cmake --build build

ctest --test-dir build --output-on-failure   # RED until you implement src/tilemap.cpp
./build/tilemap_demo         # the window (run from project/ so it finds assets/level1.map)
```

Move with **WASD** or the **arrow keys**; you slide along walls and can't pass through solid tiles.

The complete, all-green reference is in [`project/solution/`](project/solution/) — build it the same
way (`cmake -S project/solution -B project/solution/build && cmake --build project/solution/build`)
to see `100% tests passed`.

> **Sandbox / offline note:** the first `cmake` reaches `github.com` to clone SFML and Catch2. After
> that they're cached under `build/_deps` and later builds are offline. If your machine sandboxes
> network access, run that first configure with the sandbox disabled.
>
> **If linking fails**, force the compiler: `cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++`.

---

## 6. Unit-testing the parser (the stage's testable core)

[`project/tests/test_tilemap.cpp`](project/tests/test_tilemap.cpp) is pure Catch2 over
`tilemap_core` — **no SFML, no window** — so it runs in milliseconds under `ctest`. It's the direct
payoff of keeping `Tilemap` free of rendering. It covers:

- **dimensions** — `from_text` fills `width/height/tile_size` and stores `W*H` tiles;
- **`tile_at`** — right id per cell, and out-of-range cells read as `0` without throwing;
- **`solid_at`** — world→cell mapping, including a negative coordinate that must `floor` to a
  negative cell (the `floor`-vs-truncate bug);
- **malformed input** — empty text, a non-numeric header, a non-positive dimension, too few tiles,
  too many tiles, and a non-integer tile id all throw `std::runtime_error`.

This is the discipline from stage 13 applied to a real subsystem: test the *logic* exhaustively,
leave the *rendering* to build-and-eyeball.

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a complete reference. Ordered easy → hard. The first two are standalone;
03–06 use the project's `Tilemap`, so compile them against it:

```bash
cd exercises
# standalone:
clang++ -std=c++17 -Wall -Wextra 01_grid_index.solution.cpp -o ex && ./ex
# uses the Tilemap class (compile the pure source alongside):
clang++ -std=c++17 -Wall -Wextra -I ../project/include \
    ../project/solution/src/tilemap.cpp 03_count_solid.solution.cpp -o ex && ./ex
```

Each program asserts its own results and prints `all checks passed`.

1. **`01_grid_index`** *(easy)* — store a 2D grid as one flat vector; implement `y*width + x`.
2. **`02_parse_header`** *(easy–medium)* — parse `"W H tileSize"` with `istringstream` and validate
   it (the `if (!(in >> ...))` idiom).
3. **`03_count_solid`** *(medium)* — walk the grid with `tile_at` + `is_solid_id` and count solids.
4. **`04_corner_collision`** *(medium)* — the four-corner box-vs-solid test the player uses.
5. **`05_reject_malformed`** *(medium–hard)* — wrap `from_text` in `try/catch` so bad maps report
   "invalid" instead of crashing (C++ exceptions vs Python `try/except`).
6. **`06_load_from_file`** *(medium)* — write a map to a temp file and load it back (round-trip),
   reusing the stage-10 file I/O.

---

## Your task — render and walk a tile level

In [`project/`](project/). The mini-project **combines every concept of the stage**:

- **loads** `assets/level1.map` (a 20×15, 32px level) with `Tilemap::from_file`, falling back to an
  embedded map so it runs from anywhere;
- **renders** it in one draw call via `build_tilemap_vertices` + a procedurally generated tileset;
- moves a character with WASD/arrows and **collides** with solid tiles using separate-axis
  resolution so it slides along walls;
- ships a **Catch2 test suite** for the parser, run by `ctest`.

**What you implement.** Everything above is wired up *except* the pure map logic — the four
`Tilemap` methods in [`project/src/tilemap.cpp`](project/src/tilemap.cpp) (`from_text`, `from_file`,
`tile_at`, `solid_at`) ship as `// TODO(stage 20)` stubs with placeholder returns. The starter
compiles and links, but the parser tests in
[`project/tests/test_tilemap.cpp`](project/tests/test_tilemap.cpp) are **RED** until you fill them
in. The contract for each method is in
[`project/include/game/tilemap.hpp`](project/include/game/tilemap.hpp). Implement them, run `ctest`,
and drive it to green — the SFML renderer and window are already done for you (that half is
*unverified-by-execution* on the build machine anyway; the bar is that it **builds and links**,
while the map *logic* you write is proven by the tests).

Stuck? A complete reference is in [`project/solution/`](project/solution/) — build it standalone
(`cmake -S project/solution -B project/solution/build`) to compare.

---

## Checklist before moving on

- [ ] I implemented the pure `Tilemap` methods (`from_text`/`from_file`/`tile_at`/`solid_at`) and
      drove the parser tests from RED to green.
- [ ] I can explain a tilemap as a typed, flat-stored 2D list of ints, and index it `y*width + x`.
- [ ] I parsed the text format `W H tileSize` + rows, and know what makes one malformed (and throws).
- [ ] I understand why one `sf::VertexArray` beats one sprite per tile.
- [ ] I generated the tileset procedurally with `sf::Image` (no asset files).
- [ ] I did tile collision by converting a world point to a cell and resolving X and Y separately.
- [ ] I ran the parser tests with `ctest` and understand why the pure `Tilemap` is easy to test.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — the SFML `sf::VertexArray` tilemap
tutorial and the "tile-based collision" write-ups.

Next up: [Stage 21 — Camera & viewport](../21-camera-and-viewport/) — scroll a camera across a map
this size, add a parallax background, and pin a HUD to the screen.
