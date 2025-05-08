#include "Tests/ArenaTest.hpp"
#include "Misc/Arena.hpp"
#include "Debug/Log.hpp"

using namespace te;

bool testArena()
{
	testArenaSize(1, 1);
	testArenaSize(2, 2);
	testArenaSize(4, 20);
	testDoubleFree();
	return true;
}

bool testArenaSize(size_t blockSize, size_t numAlloc)
{
	Arena<int> arena{1};
	TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Arena of blocksize %zu and %zu alloc test\n",
		blockSize, numAlloc);
	for (size_t i = 0; i < 20; ++i)
	{
		int* a = arena.allocate();
		*a = i;
		TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Allocated value: %d\n", *a);
		if (i % 4 == 0)
		{
			arena.deallocate(a);
			TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Deallocated value: %d\n", *a);
		}
	}
	TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Arena of blocksize %zu and %zu alloc test completed\n",
		blockSize, numAlloc);
	return true;
}

bool testDoubleFree()
{
	TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Double free test\n");
	Arena<float> arena2{4};
	std::vector<float*> allocatedPointers;
	for (size_t i = 0; i < 10; ++i)
	{
		allocatedPointers.push_back(arena2.allocate());
	}
	arena2.deallocate(allocatedPointers[0]);
	arena2.deallocate(allocatedPointers[0]);
	for (size_t i = 0; i < 20; ++i)
	{
		//arena2.deallocate(a);
	}
	TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Double free test completed\n");
	return true;
}