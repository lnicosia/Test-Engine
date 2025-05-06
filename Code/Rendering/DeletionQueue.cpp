#include "Rendering/DeletionQueue.hpp"

namespace te
{
	void DeletionQueue::push(std::function<void()>&& func)
	{
		deletors.emplace_back(func);
	}

	void DeletionQueue::flush()
	{
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++)
		{
			(*it)();
		}
		deletors.clear();
	}
} // namespace te