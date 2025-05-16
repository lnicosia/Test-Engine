#ifndef _QUEUE_FAMILY_INDICES_HPP_
#define _QUEUE_FAMILY_INDICES_HPP_

#include <optional>

namespace te
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transferFamily;
		std::optional<uint32_t> computeAndGraphicsFamily;

		bool isComplete() const
		{
			return graphicsFamily.has_value()
				&& presentFamily.has_value()
				&& transferFamily.has_value()
				// TODO: accept and handle only compute queue family
				&& computeAndGraphicsFamily.has_value();
		}
	};
} // namespace te


#endif // _QUEUE_FAMILY_INDICES_HPP_