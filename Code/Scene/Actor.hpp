#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_

#include "Component.hpp"

#include <vector>
#include <memory>

namespace te
{
	class Actor
	{
	public:
		Actor();
		virtual ~Actor();

		/** Components */
		void addComponent(std::shared_ptr<Component> newComponent, const Transform& transform = {});

		const std::vector<std::shared_ptr<Component>>& getComponents() const;
		std::string getName() const;

	private:
		std::vector<std::shared_ptr<Component>> components{};
		Transform worldTransform{};
		std::string name{""};
		uint32_t id{0};

	};
	
} // namespace te


#endif // _ACTOR_HPP_