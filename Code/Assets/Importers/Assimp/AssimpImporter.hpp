#ifndef _ASSIMP_IMPORTER_HPP_
#define _ASSIMP_IMPORTER_HPP_

#include "../ObjectImporter.hpp"

#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace te
{
	class AssimpImporter: public ObjectImporter
	{
	public:
		AssimpImporter();
		~AssimpImporter();

		bool readFile(const std::string& path, unsigned int flags) override;
		const std::vector<const aiTexture*>& getAiTextures();
		const std::vector<std::pair<std::string, std::string>>& getTexturePaths();
		
	private:
		void setVertexBoneData(Vertex& vertex, int id, float weight);
		void extractBoneInfo(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
		void processNode(aiNode* node, const aiScene* scene);
		void processNodeBones(aiNode* node, const aiScene* scene, const sml::mat4& parentMat);
		MeshGeometry processMesh(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(aiMaterial* mat,
			aiTextureType type, const std::string& typeName, const aiScene* scene);
		/**	Sometimes bones are missing from the original skeleton but can be
		**	found in the object's animations
		*/
		void readMissingBones(const aiScene* scene);

	private:
		std::vector<const aiTexture*> aiTextures;
		std::vector<std::pair<std::string, std::string>> texturePaths;
	};
}

#endif // _ASSIMP_IMPORTER_HPP_