// Exercise 02 — SECTIONs share setup but run independently
//
// A SECTION lets several scenarios reuse the SAME setup code without that setup
// leaking between them. Catch2 re-runs the TEST_CASE body once per leaf SECTION,
// so each section sees a FRESH copy of everything declared before it. Python has
// no direct equivalent — you'd write separate test functions or repeat setup.
//
// TODO: fill in the two SECTIONs so each starts from the same 5-element vector
// but changes it independently:
//   - "growing"  : push_back one element, then REQUIRE the size is 6
//   - "shrinking": resize to 2, then REQUIRE the size is 2
// The thing to notice: the "shrinking" section still starts at size 5, proving
// the "growing" section's push_back did NOT leak across.

#include <catch2/catch_test_macros.hpp>

#include <vector>

TEST_CASE("a vector's size changes are isolated per section", "[ex02]") {
    std::vector<int> v(5);  // shared setup: five zero-initialized elements
    REQUIRE(v.size() == 5);

    SECTION("growing") {
        // TODO: push_back a value, then REQUIRE v.size() == 6.
    }
    SECTION("shrinking") {
        // TODO: v.resize(2), then REQUIRE v.size() == 2.
    }
}
