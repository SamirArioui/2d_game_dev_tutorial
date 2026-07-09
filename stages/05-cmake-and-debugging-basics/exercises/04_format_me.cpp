// Exercise 04 — Let clang-format fix the layout
//
// This code is CORRECT (it compiles and runs) but the LAYOUT is a mess: bad indentation, braces
// in odd places, cramped spacing, `int  x`. Don't fix it by hand — that's what the formatter is
// for. clang-format is the C++ equivalent of Python's `black`: one canonical style, no arguments.
//
// The repo root has a .clang-format config; clang-format finds it automatically by searching
// upward from the file. Reformat in place:
//   clang-format -i 04_format_me.cpp
//
// Then compare to 04_format_me.solution.cpp — they should match. (Install: `brew install llvm`
// on macOS, `apt install clang-format` on Debian/Ubuntu, or see SETUP.md.)
//
// Sanity-check it still compiles after formatting:
//   clang++ -std=c++17 -Wall -Wextra 04_format_me.cpp -o ex && ./ex

#include <iostream>
int   main( ){
int hp=30 ;int mana= 10;
  if(hp>0){
std::cout<<"alive with "<<hp<<" hp and "<<mana<<" mana\n" ;
}
    else{ std::cout<< "down\n";}
return 0 ;}
