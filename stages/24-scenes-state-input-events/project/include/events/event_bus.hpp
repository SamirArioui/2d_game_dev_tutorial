#pragma once

// ===========================================================================
// A minimal EVENT BUS — the observer / publish-subscribe pattern with std::function.  (YOUR TASK)
//
// This header is the STARTER: the enum, the class, the storage and remove_dead() are given, but
// the subscribe/unsubscribe/emit/subscriber_count BODIES are stubbed with TODOs. Implementing them
// is part of this stage's mini-project; the project still compiles but tests/test_event_bus.cpp
// FAILS until you fill them in. A complete reference is in ../../solution/include/events/event_bus.hpp.
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
        // TODO(stage 24): take a fresh id (next_id_++), append {id, std::move(handler)} to
        // handlers_[type] (operator[] default-constructs an empty vector on first use), and
        // return the id.
        return 0;  // placeholder
    }

    // Stop calling the handler with this id. Must be safe even from inside a handler during
    // emit(), so don't erase while an emit() might be iterating.
    void unsubscribe(SubscriptionId id) {
        // TODO(stage 24): find the subscription with this id (in any event type) and blank its
        // handler (= nullptr). Then, if no emit() is running (emitting_ == 0), call remove_dead().
    }

    // Fire an event: call every handler subscribed to `type`, in subscription order. If nobody is
    // listening it's a harmless no-op — the emitter never needs to know who (if anyone) cares.
    void emit(EventType type, int payload = 0) {
        // TODO(stage 24): look up `type` with find() (not operator[]: don't insert). If found:
        //   - keep a REFERENCE to its vector (a handler that subscribes to a new type can rehash
        //     the map and invalidate an iterator; references to elements survive);
        //   - remember the size at the start and loop by INDEX up to it, so handlers that
        //     subscribe during emit() run from the next emit on;
        //   - ++emitting_ before the loop, --emitting_ after it, then remove_dead() if it is 0;
        //   - call a COPY of each non-empty handler (a push_back during the call may reallocate
        //     the vector and move the original away).
    }

    std::size_t subscriber_count(EventType type) const {
        // TODO(stage 24): return how many live (non-empty) handlers are subscribed to `type`.
        return 0;  // placeholder
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
