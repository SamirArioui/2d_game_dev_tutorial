#pragma once

// ===========================================================================
// ResourceManager<T> — a templated, caching loader. THE shared contract stage 26 lifts into the
// engine.
//
// The problem it solves: loading a texture from disk is slow, and two entities that use the same
// image must NOT each hold their own copy (sf::Texture can be large, and it isn't even copyable in
// a cheap way). So we load each file at most once, keep it, and hand back a reference to the same
// instance every time.
//
// Python comparison: this is memoization — `@functools.lru_cache` over "load this file", or a
// hand-rolled `cache = {}; if path not in cache: cache[path] = load(path); return cache[path]`.
//
// It works for any resource type T that offers `bool loadFromFile(const std::string&)`, which is
// exactly the SFML convention (sf::Texture, sf::Font, sf::SoundBuffer). Because it never mentions
// SFML by name, we can unit-test the caching with a lightweight fake resource — no window needed.
// ===========================================================================

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace res {

template <class T>
class ResourceManager {
public:
    // get(path): return a reference to the resource for `path`, loading it on FIRST use and
    // returning the cached instance on every subsequent call. Throws std::runtime_error if a
    // first-time load fails.
    T& get(const std::string& path) {
        auto it = cache_.find(path);
        if (it != cache_.end()) {
            return *it->second;  // cache HIT — same instance as last time, no reload
        }

        // cache MISS — load once, then store. unique_ptr means the T lives on the heap at a
        // STABLE address, so the reference we return stays valid even if the map rehashes later.
        auto resource = std::make_unique<T>();
        if (!resource->loadFromFile(path)) {
            throw std::runtime_error("ResourceManager: failed to load '" + path + "'");
        }
        T& ref = *resource;
        cache_.emplace(path, std::move(resource));
        return ref;
    }

    bool is_cached(const std::string& path) const {
        return cache_.find(path) != cache_.end();
    }

    std::size_t size() const { return cache_.size(); }

    void clear() { cache_.clear(); }  // drop everything (the destructors free the resources — RAII)

private:
    std::unordered_map<std::string, std::unique_ptr<T>> cache_;
};

}  // namespace res
