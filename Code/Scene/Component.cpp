#include "Component.hpp"

namespace te
{

	Component::Component()
	{
	}
	
	Component::~Component()
	{
	}

	void Component::setRelativeTransform(const Transform& newTransform)
	{
		relativeTransform = newTransform;
	}

	const std::vector<std::shared_ptr<Component>>& Component::getComponents() const
	{
		return components;
	}

	const std::shared_ptr<MeshInternal>& Component::getMeshInternal() const
	{
		return meshInternal;
	}

	void Component::setMeshInternal(std::shared_ptr<MeshInternal> inMesh)
	{
		meshInternal = inMesh;
	}

} // namespace te
