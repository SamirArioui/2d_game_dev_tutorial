#pragma once

// ===========================================================================
// A minimal EVENT BUS — the observer / publish-subscribe pattern with std::function.  (YOUR TASK)
//
// This header is the STARTER: the enum, the class and the handlers_ storage are given, but the
// subscribe/emit/subscriber_count BODIES are stubbed with TODOs. Implementing them is part of this
// stage's mini-project; the project still compiles but tests/test_event_bus.cpp FAILS until you
// fill them in. A complete reference is in ../../solution/include/events/event_bus.hpp.
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

    // Register a callback for an event type. Many handlers can subscribe to the same type.
    void subscribe(EventType type, Handler handler) {
        // TODO(stage 24): append `handler` to the list of handlers for `type` in handlers_
        // (handlers_[type] default-constructs an empty vector on first use). Use std::move.
    }

    // Fire an event: call every handler subscribed to `type`, in subscription order. If nobody is
    // listening it's a harmless no-op — the emitter never needs to know who (if anyone) cares.
    void emit(EventType type, int payload = 0) const {
        // TODO(stage 24): if any handlers are registered for `type`, call each of them in
        // subscription order with `payload`. Emitting an event nobody subscribed to must be a
        // harmless no-op (note this method is const, so look up without inserting).
    }

    std::size_t subscriber_count(EventType type) const {
        // TODO(stage 24): return how many handlers are subscribed to `type` (0 if none).
        return 0;  // placeholder
    }

private:
    std::unordered_map<EventType, std::vector<Handler>> handlers_;
};

}  // namespace events
