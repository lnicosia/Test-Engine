#include "MeshInternal.hpp"

#include "Assets/Importers/Assimp/AssimpImporter.hpp"
#include "Debug/Log.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

namespace te
{

	MeshInternal::MeshInternal(const std::string& path): Asset(path)
	{
	}
	
	MeshInternal::~MeshInternal()
	{
	}

	const std::string MeshInternal::getAssetType() const
	{
		return "MeshInternal";
	}

	bool MeshInternal::isVisible() const
	{
		return bIsVisible;
	}

} // namespace te