#include "Scene.hpp"

#include "Actor.hpp"

namespace te
{
	uint64_t Entity::count = 0;
	
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		for (auto& a: actors)
		{
			delete a;
		}
		for (auto& c: components)
		{
			delete c;
		}
	}

	void Scene::addComponentGeometry(const Component* component)
	{
		for (auto childComponentRef: component->getComponents())
		{
			const Component* childComponent = getComponent(childComponentRef);
			addComponentGeometry(childComponent);
		}
		internalMeshes.push_back(component->getMeshInternal());
	}

	void Scene::addActor(const EntityRef& actorRef)
	{
		bIsDirty = true;
		sceneActors.push_back(actorRef);
		const Actor* actor = getActor(actorRef);
		for (auto componentRef: actor->getComponents())
		{
			const Component* component = getComponent(componentRef);
			addComponentGeometry(component);
		}
	}

	Actor* Scene::createActor()
	{
		Actor* newActor = actorArena.allocate();
		actors.push_back(newActor);
		return newActor;
	}

	Component* Scene::createComponent()
	{
		Component* newComponent = componentArena.allocate();
		components.push_back(newComponent);
		return newComponent;
	}

	Actor* Scene::getActor(const EntityRef& actorRef)
	{
		for (auto& a: actors)
		{
			if (a->getId() == actorRef.getId())
			{
				return a;
			}
		}
		return nullptr;
	}
	
	Component* Scene::getComponent(const EntityRef& component)
	{
		for (auto& c: components)
		{
			if (c->getId() == component.getId())
			{
				return c;
			}
		}
		return nullptr;
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