#pragma once

#include <string>

// A tiny piece of reusable logic. In exercise 2 it lives in its own LIBRARY
// target so more than one program could reuse it without recompiling it.
std::string greeting(const std::string& name);
