#ifndef _COMPONENT_HPP_
#define _COMPONENT_HPP_

#include "Scene/Transform.hpp"
#include "Scene/Entity.hpp"
#include "Rendering/MeshInternal.hpp"
#include "Misc/Arena.hpp"

#include <vector>

namespace te
{
	class Component : public Entity
	{
		friend class Scene;
		friend class Arena<Component>;

	public:
		virtual ~Component();

		void setRelativeTransform(const Transform& newTransform);
		const std::vector<EntityRef>& getComponents() const;
		const std::shared_ptr<MeshInternal>& getMeshInternal() const;

		void setMeshInternal(std::shared_ptr<MeshInternal> inMesh);

	private:
		Component();
		
	private:
		Transform relativeTransform{};
		std::vector<EntityRef> components{};
		std::shared_ptr<MeshInternal> meshInternal{};

	};
	
} // namespace te


#endif // _COMPONENT_HPP_