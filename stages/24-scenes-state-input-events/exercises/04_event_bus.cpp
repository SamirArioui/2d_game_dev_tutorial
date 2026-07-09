// Exercise 04 — A publish/subscribe event bus (medium)
//
// The observer pattern: emitters ANNOUNCE that something happened; subscribers registered ahead of
// time react. Neither side knows about the other. std::function (stage 09) holds any callable,
// including a lambda that captured local state.
//
// Python comparison: handlers = defaultdict(list); handlers[event].append(cb); then call them all.
//
// TODO: implement subscribe() and emit() on the EventBus below.
//   - subscribe appends a handler for an event type.
//   - emit calls every handler for that type (in order), passing the payload; no handlers -> no-op.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_event_bus.cpp -o ex && ./ex

#include <cassert>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

enum class EventType { PointScored, GameEnded };

class EventBus {
public:
    using Handler = std::function<void(int)>;

    void subscribe(EventType type, Handler handler) {
        // TODO: append `handler` to the list for `type`.
        (void)type;
        (void)handler;
    }

    void emit(EventType type, int payload) const {
        // TODO: if there are handlers for `type`, call each with `payload`. Otherwise do nothing.
        (void)type;
        (void)payload;
    }

private:
    std::unordered_map<EventType, std::vector<Handler>> handlers_;
};

int main() {
    EventBus bus;

    int total = 0;
    bus.subscribe(EventType::PointScored, [&](int who) { total += who; });
    bus.subscribe(EventType::PointScored, [&](int who) { total += who; });  // two subscribers

    int ended = 0;
    bus.subscribe(EventType::GameEnded, [&](int) { ++ended; });

    bus.emit(EventType::PointScored, 5);  // both PointScored handlers run -> total = 10
    assert(total == 10);
    assert(ended == 0);  // GameEnded was not emitted, so its handler did not run

    bus.emit(EventType::GameEnded, 0);
    assert(ended == 1);

    std::cout << "event bus works\n";
    return 0;
}
