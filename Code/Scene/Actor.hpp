#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_

#include "Scene/Component.hpp"
#include "Scene/Entity.hpp"
#include "Misc/Arena.hpp"

#include <vector>
#include <memory>

namespace te
{
	class Actor : public Entity
	{
		friend class Scene;
		friend class Arena<Actor>;

	public:
		virtual ~Actor();

		/** Components */
		void addComponent(Component* newComponent, const Transform& transform = {});

		const std::vector<EntityRef>& getComponents() const;
		std::string getName() const;

	private:
		Actor();

	private:
		std::vector<EntityRef> components{};
		Transform worldTransform{};
		std::string name{""};

	};
	
} // namespace te


#endif // _ACTOR_HPP_