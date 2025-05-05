#include "Scene.hpp"

#include "Actor.hpp"

namespace te
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		
	}

	void Scene::addComponentGeometry(const std::shared_ptr<Component>& component)
	{
		for (auto childComponent: component->getComponents())
		{
			addComponentGeometry(childComponent);
		}
		internalMeshes.push_back(component->getMeshInternal());
	}

	void Scene::addActor(std::shared_ptr<Actor> newActor)
	{
		bIsDirty = true;
		actors.insert(newActor);
		for (auto component: newActor->getComponents())
		{
			addComponentGeometry(component);
		}
	}

	const std::vector<std::shared_ptr<MeshInternal>>& Scene::getInternalMeshes() const
	{
		return internalMeshes;
	}

	void Scene::setIsDirty(bool newDirty)
	{
		bIsDirty = newDirty;
	}

	bool Scene::isDirty() const
	{
		return bIsDirty;
	}

} // namespace te