// Exercise 03 — fixtures with TEST_CASE_METHOD
//
// A fixture bundles shared setup as a struct's data members. Catch2 constructs a
// FRESH fixture instance for every TEST_CASE_METHOD, so tests can't contaminate
// one another. This is exactly pytest's fixture idea, minus the decorator.
//
// TODO: write two TEST_CASE_METHOD(BagFixture, ...) tests below:
//   1. "starts with three apples": REQUIRE(bag.count("apple") == 3)
//   2. "adding stacks the count" : bag.add("apple", 2), then
//        REQUIRE(bag.count("apple") == 5) and REQUIRE(bag.distinct() == 2)
// Because each test gets its own fixture, test 1 does not see test 2's changes.

#include <catch2/catch_test_macros.hpp>

#include <string>
#include <unordered_map>

// --- tiny type under test ---------------------------------------------------
class Bag {
public:
    void add(const std::string& name, int n) { counts_[name] += n; }
    int count(const std::string& name) const {
        auto it = counts_.find(name);
        return it == counts_.end() ? 0 : it->second;
    }
    int distinct() const { return static_cast<int>(counts_.size()); }

private:
    std::unordered_map<std::string, int> counts_;
};

// --- the fixture: its constructor is the shared "arrange" step --------------
struct BagFixture {
    Bag bag;
    BagFixture() {
        bag.add("apple", 3);
        bag.add("key", 1);
    }
};

// TODO: add your two TEST_CASE_METHOD(BagFixture, "...", "[ex03]") tests here.
