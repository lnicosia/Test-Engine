#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "Actor.hpp"
#include "Rendering/MeshInternal.hpp"

#include <set>

namespace te
{
	class Scene
	{

	public:

		Scene();
		~Scene();

		void addActor(std::shared_ptr<Actor> newActor);

		const std::vector<std::shared_ptr<MeshInternal>>& getInternalMeshes() const;

		bool isDirty() const;
		void setIsDirty(bool newDirty);
		
	private:

		void addComponentGeometry(const std::shared_ptr<Component>& component);

	private:

		std::set<std::shared_ptr<Actor>> actors{};
		std::vector<std::shared_ptr<MeshInternal>> internalMeshes{};

		bool bIsDirty = false;
	};

} // namespace te


#endif // _SCENE_HPP_