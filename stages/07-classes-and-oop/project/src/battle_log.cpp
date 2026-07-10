// YOUR TASK — implement the BattleLog methods declared in battle_log.hpp.
// A complete reference is in ../solution/src/battle_log.cpp.
#include "battle_log.hpp"

#include <iostream>
#include <utility>   // std::move

// The member-initializer list (title_) is given; you fill in the ACQUIRE step in the body.
BattleLog::BattleLog(std::string title) : title_(std::move(title)) {
    // TODO(stage 07): ACQUIRE the resource — announce the log is open by printing an opening
    // banner "=== <title_> ===\n". (In a real engine this might instead open a file.)
}

BattleLog::~BattleLog() {
    // TODO(stage 07): RELEASE the resource — this is the RAII payload. Print the closing summary
    // "=== <title_> closed (<entries_> entries logged) ===\n". Nothing calls this by hand: it
    // runs automatically when the BattleLog leaves scope, which is the whole point of RAII.
}

void BattleLog::record(const std::string& line) {
    // TODO(stage 07): increment entries_ and print "  <line>\n".
}
