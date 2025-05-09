#ifndef _ARENA_HPP_
#define _ARENA_HPP_

#include "Debug/Log.hpp"
#include "Debug/Assert.hpp"

#include <list>

// Size in number of elements
constexpr std::size_t DEFAULT_ARENA_SIZE = 1024;

namespace te
{
	template< typename T >
	class Arena
	{

		class MemoryBlock
		{

			friend class Arena;

		public:
			
			bool isFull() const
			{
				return currentIndex >= capacity && freeNodes.empty();
			}

		private:
			MemoryBlock()
			{
				data = new T[capacity];
			}
			MemoryBlock(size_t capacity)
				: capacity(capacity)
			{
				data = new T[capacity];
			}

		private:

			T* data{};

			std::size_t currentIndex{ 0 };
			std::size_t capacity { DEFAULT_ARENA_SIZE };
			std::list<T*> freeNodes{};
		};
		
	public:
		Arena() = default;
		Arena(const Arena&) = delete;
		Arena(Arena&&) = delete;
		Arena& operator=(const Arena&) = delete;
		Arena& operator=(Arena&&) = delete;
		Arena(size_t blockSize)
			: blockSize(blockSize)
		{
			TE_ASSERT(blockSize > 0);
		}

		virtual ~Arena()
		{
			for (auto& block : memoryBlocks)
			{
				delete[] block.data;
			}
		}

		template<typename... Args>
		T* allocate(Args&&... args)
		{
			for (auto& block : memoryBlocks)
			{
				//TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Current block size: %zu/%zu\n", block.currentIndex, block.capacity);
				if (!block.freeNodes.empty())
				{
					T* obj = block.freeNodes.front();
					block.freeNodes.pop_back();
					//TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Reusing free node...\n");
					return obj;
				}
				if (!block.isFull())
				{
					//TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Memory available in block, using it...\n");
					T* obj = block.data + block.currentIndex++;
					*obj = T(std::forward<Args>(args)...);
					return obj;
				}
			}
			//TE_LOG(TE_GENERAL_LOG, TE_DISPLAY, "Allocating new memory block of %zu items...\n", blockSize);
			MemoryBlock newBlock{blockSize};
			T* obj = newBlock.data + newBlock.currentIndex++;
			memoryBlocks.push_back(newBlock);
			*obj = T(std::forward<Args>(args)...);
			return obj;
		}

		void deallocate(T* obj)
		{
			for (auto& block : memoryBlocks)
			{
				if (obj >= block.data && obj < block.data + DEFAULT_ARENA_SIZE)
				{
					block.freeNodes.push_back(obj);
					return;
				}
			}
		}

		void clear()
		{
			for (auto& block : memoryBlocks)
			{
				delete[] block.data;
			}
			memoryBlocks.clear();
		}

	private:

		std::list<MemoryBlock> memoryBlocks{};
		const std::size_t blockSize{ DEFAULT_ARENA_SIZE };
	};
} // namespace te

#endif // _ARENA_HPP_