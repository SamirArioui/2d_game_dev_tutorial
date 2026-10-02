#pragma once

// ===========================================================================
// A minimal EVENT BUS — the observer / publish-subscribe pattern with std::function.
//
// Instead of the scoring code reaching directly into the HUD, the audio, and the state machine,
// it just EMITS "a point was scored" and forgets about it. Interested parties SUBSCRIBE a callback
// ahead of time. This decouples "something happened" from "who reacts to it" — the same idea as
// Qt signals/slots, JS `addEventListener`, or Python's callback lists.
//
// Python comparison: a `defaultdict(list)` of callables —
//     handlers[event].append(cb)  ...  for cb in handlers[event]: cb(payload)
//
// std::function (stage 09) is C++'s type-erased "any callable" — it can hold a free function, a
// lambda, or a lambda that captured state, all behind one type.
// ===========================================================================

#include <algorithm>
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace events {

enum class EventType { PointScored, BallOut, GameStarted, GameEnded };

class EventBus {
public:
    // A handler takes one int payload (e.g. which player scored). Kept deliberately simple; a
    // real bus might pass a variant or a small struct per event type.
    using Handler = std::function<void(int)>;

    // subscribe() returns an id you pass to unsubscribe() later. Unsubscribe before the object a
    // handler captured goes away — e.g. in the destructor of a scene that subscribed `[this]` —
    // or the bus would call into a dead object (a dangling capture).
    using SubscriptionId = std::size_t;

    // Register a callback for an event type. Many handlers can subscribe to the same type.
    SubscriptionId subscribe(EventType type, Handler handler) {
        const SubscriptionId id = next_id_++;
        handlers_[type].push_back({id, std::move(handler)});
        return id;
    }

    // Stop calling the handler with this id. Safe even from inside a handler during emit(): the
    // entry is only blanked now and physically removed once no emit() is running.
    void unsubscribe(SubscriptionId id) {
        for (auto& [type, subs] : handlers_) {
            for (auto& sub : subs) {
                if (sub.id == id) {
                    sub.handler = nullptr;
                }
            }
        }
        if (emitting_ == 0) {
            remove_dead();
        }
    }

    // Fire an event: call every handler subscribed to `type`, in subscription order. If nobody is
    // listening it's a harmless no-op — the emitter never needs to know who (if anyone) cares.
    //
    // Handlers may subscribe or unsubscribe WHILE we are iterating, so:
    //   - we hold a reference to the vector, not a map iterator (subscribing to a NEW type can
    //     rehash the map, which invalidates iterators; references to elements survive);
    //   - we loop by index up to the size at the START, so new subscribers wait for the next emit;
    //   - we call a COPY of each handler: a push_back may reallocate the vector and move the
    //     std::function we'd otherwise be running out from under ourselves.
    void emit(EventType type, int payload = 0) {
        auto it = handlers_.find(type);
        if (it == handlers_.end()) {
            return;
        }
        std::vector<Subscription>& subs = it->second;
        const std::size_t count = subs.size();
        ++emitting_;
        for (std::size_t i = 0; i < count; ++i) {
            Handler handler = subs[i].handler;
            if (handler) {  // blank = unsubscribed
                handler(payload);
            }
        }
        --emitting_;
        if (emitting_ == 0) {
            remove_dead();
        }
    }

    std::size_t subscriber_count(EventType type) const {
        auto it = handlers_.find(type);
        if (it == handlers_.end()) {
            return 0;
        }
        std::size_t count = 0;
        for (const auto& sub : it->second) {
            if (sub.handler) {
                ++count;
            }
        }
        return count;
    }

private:
    struct Subscription {
        SubscriptionId id;
        Handler handler;  // empty once unsubscribed
    };

    // Erase blanked entries (erase-remove idiom). Only called when no emit() is iterating.
    void remove_dead() {
        for (auto& [type, subs] : handlers_) {
            subs.erase(std::remove_if(subs.begin(), subs.end(),
                                      [](const Subscription& s) { return !s.handler; }),
                       subs.end());
        }
    }

    std::unordered_map<EventType, std::vector<Subscription>> handlers_;
    SubscriptionId next_id_{1};
    int emitting_{0};  // > 0 while emit() is running (a handler may emit, so it can nest)
};

}  // namespace events
