#pragma once

// ============================================================================
// eng::ResourceManager<T> — load-once, cache-by-path resource cache.
//
// Textures, fonts and sound buffers are expensive to load and must OUTLIVE
// every sprite/text/sound that refers to them (SFML keeps a pointer, not a
// copy). A resource manager solves both problems: it owns one copy per path and
// hands out long-lived references.
//
// It is a TEMPLATE with no SFML dependency: it works for ANY type `T` that is
// default-constructible and has a `bool loadFromFile(const std::string&)`
// method. sf::Texture, sf::Font and sf::SoundBuffer all satisfy that (duck
// typing, checked at compile time when you instantiate it) — and so can a fake
// type in a unit test, which is exactly how we test the caching without SFML.
//
// Python note: like a dict-backed memoiser, e.g.
//     @lru_cache
//     def get(path): return load(path)
// except here we also GUARANTEE the address is stable (see unique_ptr below).
// ============================================================================

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

namespace eng {

template <class T>
class ResourceManager {
public:
    // Return the resource for `path`, loading it on the FIRST request and
    // reusing the cached copy afterwards. The returned reference stays valid for
    // the manager's lifetime.
    //
    // Why store unique_ptr<T> and not T? Because an unordered_map may REHASH as
    // it grows, moving its elements. A T stored by value would change address; a
    // pointer-to-heap keeps the T itself put, so references we handed out earlier
    // never dangle.
    T& get(const std::string& path) {
        auto it = cache_.find(path);
        if (it != cache_.end()) {
            return *it->second;  // cache hit — no load
        }

        auto resource = std::make_unique<T>();
        // If the file is missing we STILL cache the (empty) resource rather than
        // retry a broken path every frame. SFML draws an empty texture as a white
        // box, which is a visible "asset missing" signal.
        resource->loadFromFile(path);
        ++load_count_;

        T& ref = *resource;
        cache_.emplace(path, std::move(resource));
        return ref;
    }

    bool contains(const std::string& path) const { return cache_.count(path) > 0; }
    std::size_t size() const { return cache_.size(); }

    // How many times we actually LOADED (as opposed to served from cache).
    // Exposed mainly so a unit test can prove that a second get() is a cache hit.
    std::size_t load_count() const { return load_count_; }

    void clear() {
        cache_.clear();
        load_count_ = 0;
    }

private:
    std::unordered_map<std::string, std::unique_ptr<T>> cache_;
    std::size_t load_count_{0};
};

}  // namespace eng
