#pragma once

// ===========================================================================
// ResourceManager<T> — a templated, caching loader. THE shared contract stage 26 lifts into the
// engine.                                                                        (YOUR TASK)
//
// This header is the STARTER: the class, every method SIGNATURE and the cache_ storage are given,
// but the method BODIES are stubbed with TODOs and placeholder returns. Implementing them is part
// of this stage's mini-project; the project still compiles but tests/test_resource_manager.cpp
// FAILS until you fill them in. A complete reference is in
// ../../solution/include/res/resource_manager.hpp — try it yourself first.
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
        // TODO(stage 25): implement load-on-first-use + caching.
        //   - cache HIT: if `path` is already in cache_, return a reference to the SAME instance
        //     (no reload).
        //   - cache MISS: make_unique<T>(), call resource->loadFromFile(path); on failure throw
        //     std::runtime_error (and do NOT cache it). On success, store the unique_ptr in cache_
        //     and return a reference to the owned T. Storing via unique_ptr keeps the T at a STABLE
        //     heap address, so the reference stays valid even if the map rehashes later.
        static T placeholder{};  // placeholder so the starter compiles; replace with the real load
        return placeholder;
    }

    bool is_cached(const std::string& path) const {
        // TODO(stage 25): return whether `path` is currently in the cache.
        return false;  // placeholder
    }

    std::size_t size() const {
        // TODO(stage 25): return how many resources are cached.
        return 0;  // placeholder
    }

    void clear() {
        // TODO(stage 25): drop every cached resource (the unique_ptr destructors free them — RAII).
    }

private:
    std::unordered_map<std::string, std::unique_ptr<T>> cache_;
};

}  // namespace res
