# Stage 25 — Resource manager & data-driven design

**Goal:** stop loading the same texture ten times, and stop hardcoding your levels. You'll build a
templated, caching **`ResourceManager<T>`** that loads each asset once and hands back the same
instance forever after, and you'll learn **data-driven design** — describing a level in a text file
the game reads at run time, instead of baking it into C++.

**New concepts:** a **class template with caching** (`ResourceManager<T>`) · **load-on-first-use +
memoization** · returning a **reference** to a cached resource (and why not a copy) · **RAII
ownership** of resources via `unique_ptr` · **data-driven design**: parse a config/level file
(reusing stage-10 file I/O) into plain structs. (*Python comparisons:* `functools.lru_cache` /
a `dict` cache, and loading a JSON/YAML config into objects.)

`ResourceManager<T>` is the **shared contract** the engine (stage 26) adopts wholesale, so its
signature — `T& get(const std::string& path)` with caching — is fixed on purpose.

---

## 1. The problem: loading is slow, and copies are worse

Reading a PNG off disk and uploading it to the GPU is *expensive*. If every wall tile calls
`texture.loadFromFile("grass.png")`, a level with 200 grass tiles does that work 200 times and
stores 200 identical textures. That's slow to load and wasteful of memory.

Worse, `sf::Texture` isn't something you want to copy around: it can be large, and copying it means
another GPU upload. So handing each entity *its own* texture value is doubly bad.

The fix is the same one you'd reach for in Python: **cache** the result of an expensive lookup, and
**share** it.

### Python comparison

```python
@functools.lru_cache(maxsize=None)          # memoize: run the body once per distinct path
def load_texture(path):
    return Texture.from_file(path)          # slow work happens on first call only

a = load_texture("grass.png")
b = load_texture("grass.png")               # cached: same object, no reload
assert a is b
```

Our `ResourceManager<T>` is this idea as a small C++ class: a `dict` from path to loaded resource,
with "load it if missing" logic in one place.

---

## 2. `ResourceManager<T>`

```cpp
// res/resource_manager.hpp  (header-only template, SFML-free)
template <class T>
class ResourceManager {
    std::unordered_map<std::string, std::unique_ptr<T>> cache_;
public:
    T& get(const std::string& path) {
        auto it = cache_.find(path);
        if (it != cache_.end()) {
            return *it->second;                    // cache HIT: same instance, no reload
        }
        auto resource = std::make_unique<T>();     // cache MISS: load once...
        if (!resource->loadFromFile(path)) {
            throw std::runtime_error("failed to load '" + path + "'");
        }
        T& ref = *resource;
        cache_.emplace(path, std::move(resource)); // ...and keep it
        return ref;
    }
    bool is_cached(const std::string& path) const;
    std::size_t size() const;
    void clear();
};
```

Three design decisions worth dwelling on:

- **It's a template on `T`.** One class serves `ResourceManager<sf::Texture>`,
  `ResourceManager<sf::Font>`, `ResourceManager<sf::SoundBuffer>` — every SFML resource offers
  `bool loadFromFile(const std::string&)`, which is all `get` requires. (This is *duck typing at
  compile time*, stage 08: any `T` with the right method works, checked when the template is
  instantiated.)
- **It returns `T&`, not `T`.** Returning a reference means the caller shares the one cached
  instance. Returning by value would copy the texture on every call — defeating the whole point.
  The lifetime is safe because the manager owns the resource for as long as it lives.
- **It stores `std::unique_ptr<T>`, not `T`.** Two reasons. First, RAII ownership (stages 11/07):
  when the manager is destroyed — or you call `clear()` — the `unique_ptr` destructors free every
  resource automatically. Second, and subtly: an `unordered_map` may **rehash** as it grows, moving
  its elements. If we stored `T` by value, a `T&` we handed out earlier could dangle after a
  rehash. A `unique_ptr` keeps the `T` at a **stable heap address**, so references stay valid.

> **`const`-correctness (course thread).** `is_cached` and `size` are `const` — querying the cache
> doesn't change it. `get` is non-const because a miss mutates the cache.

### Why this is unit-testable without a window

`ResourceManager<T>` never names SFML. So [`tests/test_resource_manager.cpp`](project/tests/test_resource_manager.cpp)
hands it a tiny **fake resource** whose `loadFromFile` just increments a counter, and asserts the
things that matter: *load happens exactly once*, *the same instance comes back*, *a failed load
throws and isn't cached*. Proving caching needs no textures and no display.

---

## 3. Data-driven design

Hardcoding `spawn_wall(300, 150)` means editing C++ and recompiling to move a wall. **Data-driven
design** flips that: the level is *data* the program reads at run time.

This stage carries a self-contained level file, [`project/assets/level.txt`](project/assets/level.txt):

```
title  Resource Manager Demo
size   640 480

entity   0 416 tiles/grass.png     # '#' starts a comment; blank lines are ignored
entity  32 416 tiles/grass.png     # grass.png appears many times...
entity 256 384 tiles/wall.png
entity 320 200 player.png
```

A small parser (in [`src/level_config.cpp`](project/src/level_config.cpp), reusing the file I/O from
stage 10) turns it into plain structs:

```cpp
struct EntitySpec { std::string texture; float x, y; };
struct LevelConfig { std::string title; int width, height; std::vector<EntitySpec> entities; };

LevelConfig parse_level(std::istream& in);            // works on ANY stream -> testable
LevelConfig load_level(const std::string& path);      // opens the file, then parse_level
```

Designers (or you, next week) tweak the level in a text editor — no rebuild. Unknown directives are
skipped rather than fatal, so the format can grow without breaking old files.

### Python comparison

This is loading a config into objects, just with a tinier format than JSON/YAML:

```python
cfg = json.load(open("level.json"))          # or yaml, toml, ...
for e in cfg["entities"]:
    spawn(e["texture"], e["x"], e["y"])
```

We use a whitespace-delimited text format precisely so the parser stays a few lines and *teachable*
— the concept (data outside the code) is identical. And because `parse_level` takes an
`std::istream&`, the tests feed it an in-memory `std::istringstream` — no temp files.

---

## 4. Putting them together

The two ideas meet in the mini-project's spawn loop: parse the level into `EntitySpec`s, then pull
each entity's texture **through the manager**. Because five grass tiles share one path, the manager
loads `grass.png` **once** and every grass sprite points at that single instance:

```cpp
res::ResourceManager<sf::Texture> textures;
for (const auto& spec : level.entities) {
    sf::Texture& tex = textures.get(spec.texture);   // duplicates are cached
    sprites.emplace_back(tex);                        // sprite refers to the shared texture
}
// e.g. 8 entities, but textures.size() == 3 unique textures actually loaded.
```

A production touch worth copying: if a texture fails to load, the demo falls back to a bright
**magenta "missing texture"** placeholder instead of crashing — a typo in the data file becomes
*visible* rather than fatal.

---

## Building and running

The mini-project has its own [`project/CMakeLists.txt`](project/CMakeLists.txt). **The first
configure clones Catch2 (and, for the app, SFML 2.6.1) from GitHub — network required.**

```bash
cd project

# --- Fast path: tests only (no SFML, no display) ---
cmake -S . -B build -DBUILD_APP=OFF
cmake --build build
ctest --test-dir build --output-on-failure        # 8 tests — RED until you implement the TODOs

# --- Full path: also build the SFML demo ---
cmake -S . -B build
cmake --build build
./build/tilemap_demo        # run from THIS folder so "assets/..." resolves
```

The bundled `assets/` (a `level.txt` and three tiny solid-colour PNGs) makes the demo
self-contained. Edit `level.txt` and re-run — no rebuild.

Exercises are a separate, SFML-free project:

```bash
cd exercises
cmake -S . -B build && cmake --build build
ctest --test-dir build --output-on-failure
# ...or by hand:  clang++ -std=c++17 -Wall -Wextra 02_resource_cache.cpp -o ex && ./ex
```

> **Troubleshooting.** Link errors → add `-DCMAKE_CXX_COMPILER=/usr/bin/clang++`. macOS *"Could NOT
> find OpenGL"* during the SFML configure → pass `-DCMAKE_OSX_SYSROOT="$(xcrun --show-sdk-path)"`.

---

## Exercises

In [`exercises/`](exercises/). Starter `NN_name.cpp` + reference `NN_name.solution.cpp`, easy → hard.

1. **`01_memoize`** *(easy)* — cache an expensive function's results in an `unordered_map`; prove
   the slow work runs once per key. The plain idea under the manager.
2. **`02_resource_cache`** *(medium)* — implement `ResourceManager<T>::get`: load-once, return the
   same instance, count loads with a fake resource.
3. **`03_parse_line`** *(medium)* — parse one `entity x y texture` line into a struct with
   `std::istringstream`.
4. **`04_parse_config`** *(medium–hard)* — parse a whole config stream: strip comments, skip blanks,
   read `size` and `entity` directives, ignore unknown ones.
5. **`05_data_driven_spawn`** *(hard)* — combine them: spawn entities from parsed specs, pulling
   textures through the cache so shared textures load once.

---

## Your task — a data-driven screen through the ResourceManager

**Your task.** [`project/`](project/) is a **starter**: the struct declarations, the demo in
[`src/main.cpp`](project/src/main.cpp), the `assets/` and the tests are all given, but two pieces
are stubbed with `TODO`s for you to implement:

- **`ResourceManager<T>::get`** (plus `is_cached` / `size` / `clear`) in
  [`include/res/resource_manager.hpp`](project/include/res/resource_manager.hpp) — load-on-first-use
  + caching, returning `T&` (note it's a header template, so the bodies live in the header);
- **`parse_level`** in [`src/level_config.cpp`](project/src/level_config.cpp) — the data-driven
  config parser (`trim` and the `load_level` file wrapper are provided).

The starter compiles and links, but `ctest` is **RED** until you fill in the `TODO`s — work through
[`tests/test_resource_manager.cpp`](project/tests/test_resource_manager.cpp) and
[`tests/test_level_config.cpp`](project/tests/test_level_config.cpp) until green. When done it
**combines every concept in the stage**: `main` reads `assets/level.txt` via `load_level`
(data-driven design, file I/O), builds a sprite per `EntitySpec` fetching each texture through
`res::ResourceManager<sf::Texture>` so repeats are cached, shares one texture instance across many
sprites (reference return, `unique_ptr` ownership), and degrades to a magenta placeholder on a bad
path.

Stuck? A complete reference is in [`project/solution/`](project/solution/) — build it standalone
(it has its own `CMakeLists.txt`) and compare:

```bash
cmake -S project/solution -B project/solution/build -DBUILD_APP=OFF
cmake --build project/solution/build
ctest --test-dir project/solution/build --output-on-failure   # all green
```

Build + link of your version is verified; on-screen rendering is unverified-by-execution (CI has no
display), but the asset pipeline and caching are exercised by the tests.

---

## Checklist before moving on

- [ ] I implemented the mini-project starter (`ResourceManager<T>` + `parse_level`) so `ctest` is
      green.
- [ ] I can explain why loading each asset once and sharing it (by reference) beats copying.
- [ ] I implemented `get` with load-on-first-use caching and understand why it returns `T&`.
- [ ] I know why the cache stores `unique_ptr<T>` (RAII cleanup + stable address across rehash).
- [ ] I can describe data-driven design and why the level lives in a file, not in C++.
- [ ] My parser reads from an `istream` so it's testable without touching disk.
- [ ] I proved, with a fake resource, that the manager loads once and caches.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md).

Next up: [Stage 26 — Extract the engine](../26-extract-the-engine/) — where `Scene`, the
`ResourceManager`, and the component model move into `namespace eng` as `libengine`.
