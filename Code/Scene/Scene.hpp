#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "Scene/Actor.hpp"
#include "Scene/Entity.hpp"
#include "Rendering/MeshInternal.hpp"
#include "Debug/Exception.hpp"
#include "Misc/Arena.hpp"

#include <set>

namespace te
{
	class Scene
	{

	public:

		Scene();
		~Scene();

		void addActor(const EntityRef& actorRef);
		Actor* createActor();
		Component* createComponent();

		const std::vector<std::shared_ptr<MeshInternal>>& getInternalMeshes() const;

		bool isDirty() const;
		void setIsDirty(bool newDirty);
		
	private:

		void addComponentGeometry(const Component* component);

		Actor* getActor(const EntityRef& actorRef);
		Component* getComponent(const EntityRef& componentRef);

	private:

		/** Scene graph */
		std::vector<EntityRef> sceneActors{};

		/** Scene data
		 */
		Arena<Actor> actorArena{};
		Arena<Component> componentArena{};
		std::vector<Actor*> actors{};
		std::vector<Component*> components{};

		/** Draw data */
		std::vector<std::shared_ptr<MeshInternal>> internalMeshes{};

		bool bIsDirty = false;
	};

} // namespace te


#endif // _SCENE_HPP_