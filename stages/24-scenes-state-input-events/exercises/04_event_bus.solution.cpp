// Exercise 04 — A publish/subscribe event bus (solution)
#include <cassert>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

enum class EventType { PointScored, GameEnded };

class EventBus {
public:
    using Handler = std::function<void(int)>;

    void subscribe(EventType type, Handler handler) {
        handlers_[type].push_back(std::move(handler));
    }

    void emit(EventType type, int payload) const {
        auto it = handlers_.find(type);
        if (it == handlers_.end()) {
            return;
        }
        for (const auto& handler : it->second) {
            handler(payload);
        }
    }

private:
    std::unordered_map<EventType, std::vector<Handler>> handlers_;
};

int main() {
    EventBus bus;

    int total = 0;
    bus.subscribe(EventType::PointScored, [&](int who) { total += who; });
    bus.subscribe(EventType::PointScored, [&](int who) { total += who; });

    int ended = 0;
    bus.subscribe(EventType::GameEnded, [&](int) { ++ended; });

    bus.emit(EventType::PointScored, 5);
    assert(total == 10);
    assert(ended == 0);

    bus.emit(EventType::GameEnded, 0);
    assert(ended == 1);

    std::cout << "event bus works\n";
    return 0;
}
