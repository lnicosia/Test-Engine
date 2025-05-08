#include "Actor.hpp"


namespace te
{
	Actor::Actor() : Entity{}
	{
	}
	
	Actor::~Actor()
	{
	}

	void Actor::addComponent(Component* newComponent, const Transform& transform)
	{
		components.emplace_back(newComponent->getId());
		newComponent->setRelativeTransform(transform);
	}

	const std::vector<EntityRef>& Actor::getComponents() const
	{
		return components;
	}

	std::string Actor::getName() const
	{
		return name;
	}

} // namespace te
