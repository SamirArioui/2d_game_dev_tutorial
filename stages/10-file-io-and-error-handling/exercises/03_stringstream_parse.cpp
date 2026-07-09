// Exercise 03 — Parsing lines with std::stringstream (medium)
//
// A file gives you a std::string per line; you usually need to break that string
// into typed FIELDS. std::stringstream wraps a string and lets you use the same
// >> and getline operators you use on files:
//
//   - ss >> x            reads whitespace-separated tokens and converts by type
//                        (int, double, string). Like Python's line.split() plus int().
//   - getline(ss, f, d)  reads up to the delimiter d (handy for CSV / '|'-separated
//                        fields, and for names that contain spaces).
//
// This exercise parses two line formats used by the mini-project's save file.
//
// Fill in the TODOs. Expected output:
//   stats: hp=30 attack=12 defense=8
//   item: name='Health Potion' value=25 count=3
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 03_stringstream_parse.cpp -o ex && ./ex

#include <iostream>
#include <sstream>
#include <string>

int main() {
    // --- format A: three whitespace-separated ints ---
    std::string stats_line = "30 12 8";
    std::stringstream ss(stats_line);

    int hp = 0, attack = 0, defense = 0;
    // TODO 1: extract the three ints in order with  ss >> hp >> attack >> defense;
    //         Then print: stats: hp=30 attack=12 defense=8

    // --- format B: '|'-delimited fields, where the name may contain spaces ---
    // Layout:  name|value|count
    std::string item_line = "Health Potion|25|3";
    std::stringstream is(item_line);

    std::string name;
    std::string value_str;
    std::string count_str;
    // TODO 2: read the three '|'-separated fields with getline(is, field, '|'):
    //         getline(is, name, '|');  getline(is, value_str, '|');  getline(is, count_str);
    //         (the last getline has no delimiter, so it takes the rest of the string.)

    // TODO 3: convert value_str and count_str to int with std::stoi, then print:
    //         item: name='Health Potion' value=25 count=3

    return 0;
}
