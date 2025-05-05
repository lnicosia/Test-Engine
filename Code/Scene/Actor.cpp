#include "Actor.hpp"


namespace te
{
	
	Actor::Actor()
	{
	}
	
	Actor::~Actor()
	{
	}

	void Actor::addComponent(std::shared_ptr<Component> newComponent, const Transform& transform)
	{
		components.push_back(newComponent);
		newComponent->setRelativeTransform(transform);
	}

	const std::vector<std::shared_ptr<Component>>& Actor::getComponents() const
	{
		return components;
	}

	std::string Actor::getName() const
	{
		return name;
	}

} // namespace te
