#ifndef _DELETION_QUEUE_HPP_
#define _DELETION_QUEUE_HPP_

#include <deque>
#include <functional>

namespace te
{
	struct DeletionQueue
	{

	public:
		void push(std::function<void()>&& func);
		void flush();
		
	public:
		std::deque<std::function<void()>> deletors;
	};
} // namespace te

#endif // _DELETION_QUEUE_HPP