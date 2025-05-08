#ifndef ARENA_TEST_HPP
#define ARENA_TEST_HPP

#include "Misc/Arena.hpp"

bool testArena();
bool testArenaSize(size_t blockSize, size_t numAlloc);
bool testDoubleFree();

#endif // ARENA_TEST_HPP