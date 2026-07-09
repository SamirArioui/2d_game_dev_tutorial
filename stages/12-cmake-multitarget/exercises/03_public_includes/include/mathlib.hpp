#pragma once

// Declared here, in a dedicated include/ directory that is SEPARATE from the
// sources in src/. Because the header no longer sits next to the .cpp files that
// use it, CMake must be told where to find it — that is what this exercise adds.
int add(int a, int b);
