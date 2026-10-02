// Tests for the publish/subscribe event bus.
#include <catch2/catch_test_macros.hpp>

#include "events/event_bus.hpp"

using events::EventBus;
using events::EventType;

TEST_CASE("emit calls every subscriber for that event, with the payload", "[events]") {
    EventBus bus;

    int calls = 0;
    int last_payload = -1;
    bus.subscribe(EventType::PointScored, [&](int who) {
        ++calls;
        last_payload = who;
    });

    // A second, independent subscriber to the SAME event.
    int mirror = 0;
    bus.subscribe(EventType::PointScored, [&](int who) { mirror = who; });

    bus.emit(EventType::PointScored, 2);

    REQUIRE(calls == 1);
    REQUIRE(last_payload == 2);  // payload delivered
    REQUIRE(mirror == 2);        // both subscribers ran
    REQUIRE(bus.subscriber_count(EventType::PointScored) == 2);
}

TEST_CASE("emitting an event nobody subscribed to is a harmless no-op", "[events]") {
    EventBus bus;
    REQUIRE(bus.subscriber_count(EventType::GameEnded) == 0);
    REQUIRE_NOTHROW(bus.emit(EventType::GameEnded, 0));  // must not crash
}

TEST_CASE("different event types are isolated", "[events]") {
    EventBus bus;
    int scored = 0;
    int ended = 0;
    bus.subscribe(EventType::PointScored, [&](int) { ++scored; });
    bus.subscribe(EventType::GameEnded, [&](int) { ++ended; });

    bus.emit(EventType::PointScored);

    REQUIRE(scored == 1);
    REQUIRE(ended == 0);  // the GameEnded handler was NOT called
}

TEST_CASE("unsubscribe stops a handler from being called", "[events]") {
    EventBus bus;
    int a = 0;
    int b = 0;
    const auto id_a = bus.subscribe(EventType::PointScored, [&](int) { ++a; });
    bus.subscribe(EventType::PointScored, [&](int) { ++b; });

    bus.unsubscribe(id_a);
    bus.emit(EventType::PointScored);

    REQUIRE(a == 0);  // gone
    REQUIRE(b == 1);  // the other subscriber is untouched
    REQUIRE(bus.subscriber_count(EventType::PointScored) == 1);
}

TEST_CASE("handlers may subscribe and unsubscribe during emit", "[events]") {
    EventBus bus;
    int late_calls = 0;
    int other_calls = 0;
    EventBus::SubscriptionId self_id = 0;

    // This handler adds MANY subscribers mid-emit (enough to force the vector to reallocate) and
    // also unsubscribes itself. Without the index loop + handler copy that is undefined behaviour.
    self_id = bus.subscribe(EventType::PointScored, [&](int) {
        for (int i = 0; i < 32; ++i) {
            bus.subscribe(EventType::PointScored, [&](int) { ++late_calls; });
        }
        bus.subscribe(EventType::GameEnded, [&](int) { ++other_calls; });  // a NEW type: may rehash
        bus.unsubscribe(self_id);
    });

    bus.emit(EventType::PointScored);
    REQUIRE(late_calls == 0);  // subscribers added during an emit start with the NEXT emit
    REQUIRE(bus.subscriber_count(EventType::PointScored) == 32);  // the self-unsubscribe applied

    bus.emit(EventType::PointScored);
    REQUIRE(late_calls == 32);
}
