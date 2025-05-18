#include "MeshInternal.hpp"

#include "Assets/Importers/Assimp/AssimpImporter.hpp"
#include "Debug/Log.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

namespace te
{

	MeshInternal::MeshInternal(const std::string& path): Asset(path),
		localTransform(1.0f), worldTransform(1.0f)
	{
		
	}
	
	MeshInternal::~MeshInternal()
	{
	}

	const std::string MeshInternal::getAssetType() const
	{
		return "MeshInternal";
	}

	MeshBounds MeshInternal::getBounds() const
	{
		return bounds;
	}

	sml::mat4 MeshInternal::getLocalTransform() const
	{
		return localTransform;
	}

	sml::mat4 MeshInternal::getWorldTransform() const
	{
		return worldTransform;
	}

	void MeshInternal::updateTransform(const sml::mat4& parentMat)
	{
		worldTransform = parentMat * localTransform;
	}

	bool MeshInternal::isVisible() const
	{
		return bIsVisible;
	}

	bool MeshInternal::shouldCull(const sml::mat4& viewProj) const
	{
		std::array<sml::vec3, 8> corners =
		{
			sml::vec3 { 1.f, 1.f, 1.f },
			sml::vec3 { 1.f, 1.f, -1.f },
			sml::vec3 { 1.f, -1.f, 1.f },
			sml::vec3 { 1.f, -1.f, -1.f },
			sml::vec3 { -1.f, 1.f, 1.f },
			sml::vec3 { -1.f, 1.f, -1.f },
			sml::vec3 { -1.f, -1.f, 1.f },
			sml::vec3 { -1.f, -1.f, -1.f },
		};

		sml::mat4 mat = viewProj * worldTransform;

		sml::vec3 min = { 1.5f, 1.5f, 1.5f };
		sml::vec3 max = { -1.5f, -1.5f, -1.5f };

		for (int c = 0; c < 8; c++)
		{
			sml::vec3 ext = bounds.origin + (corners[c] * bounds.extents);
			sml::vec4 v = mat * sml::vec4(ext.x, ext.y, ext.z, 1.f);

			v.x /= v.w;
			v.y /= v.w;
			v.z /= v.w;

			min = sml::min(sml::vec3(v.x, v.y, v.z), min);
			max = sml::max(sml::vec3(v.x, v.y, v.z), max);
		}


		if (min.z > 1.f || max.z < 0.f || min.x > 1.f || max.x < -1.f || min.y > 1.f || max.y < -1.f)
		{
        	return true;
		}
		else
		{
			return false;
		}
	}
} // namespace te