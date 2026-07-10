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
        handlers_[type].push_back(std::move(handler));
    }

    // Fire an event: call every handler subscribed to `type`, in subscription order. If nobody is
    // listening it's a harmless no-op — the emitter never needs to know who (if anyone) cares.
    void emit(EventType type, int payload = 0) const {
        auto it = handlers_.find(type);
        if (it == handlers_.end()) {
            return;
        }
        for (const auto& handler : it->second) {
            handler(payload);
        }
    }

    std::size_t subscriber_count(EventType type) const {
        auto it = handlers_.find(type);
        return it == handlers_.end() ? 0 : it->second.size();
    }

private:
    std::unordered_map<EventType, std::vector<Handler>> handlers_;
};

}  // namespace events
