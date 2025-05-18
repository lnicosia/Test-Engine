#include "AssimpImporter.hpp"

#include "AssimpHelpers.hpp"
#include "Rendering/MeshGeometry.hpp"
#include "Assets/AssetManager.hpp"
#include "Debug/Log.hpp"

namespace te
{
	AssimpImporter::AssimpImporter()
	{
	}

	AssimpImporter::~AssimpImporter()
	{
	}

	bool AssimpImporter::readFile(const std::string& path, unsigned int flags)
	{
		this->path = path;
		Assimp::Importer importer;
		const aiScene* scene;
		if (path.find_last_of('.') != std::string::npos && (path.substr(path.find_last_of('.')) == ".dae"
			|| path.substr(path.find_last_of('.')) == ".obj"))
		{
			scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenUVCoords);
		}
		else
		{
			scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs
				| aiProcess_GenUVCoords);
		}

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			TE_LOG(TE_RESOURCE_LOG, TE_ERROR, "assimp: %s\n", importer.GetErrorString());
			return false;
		}
		this->directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
		//processNodeBones(scene->mRootNode, scene, sml::mat4());

		//readMissingBones(scene);
		return true;
	}

	void AssimpImporter::setVertexBoneData(Vertex& vertex, int id, float weight)
	{
		/*for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			if (vertex.boneIDs[i] == -1)
			{
				vertex.boneIDs[i] = id;
				vertex.weights[i] = weight;
				//std::cout << "COUCOU" << std::endl;
				break;
			}
			//std::cout << "\tid = " << vertex.boneIDs[i] << ", weight = ";
			//std::cout << vertex.weights[i] << std::endl;
		}*/
	}

	void AssimpImporter::extractBoneInfo(std::vector<Vertex>& vertices, aiMesh* mesh,
		const aiScene* scene)
	{
		//std::cout << "Mesh " << mesh->mName.C_Str() << " has " << mesh->mNumBones << " bones" << std::endl;
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			BoneInfo bone;
			bone.id = -1;
			std::string	boneName = mesh->mBones[i]->mName.C_Str();
			bone.name = boneName;
			if (this->bones.find(boneName) == bones.end())
			{
				bone.id = nbBones;
				bone.offsetMatrix = AssimpToSmlMatrix(mesh->mBones[i]->mOffsetMatrix);
				this->bones[boneName] = bone;
				this->nbBones++;
			}
			else
			{
				bone.id = this->bones[boneName].id;
			}
			//std::cout << "Bone " << boneName <<  " id = " << bone.id << std::endl;
			aiVertexWeight* weights = mesh->mBones[i]->mWeights;
			int	nbWeights = mesh->mBones[i]->mNumWeights;
			for (int j = 0; j < nbWeights; j++)
			{
				if (weights[j].mVertexId >= vertices.size()
					|| weights[j].mWeight == 0)
				{
					continue;
				}
				//std::cout << "Vertex { " << weights[j].mVertexId << " } has the following bones:" << std::endl;
				setVertexBoneData(vertices[weights[j].mVertexId], bone.id, weights[j].mWeight);
			}
		}
	}

	MeshGeometry AssimpImporter::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		//	Vertices

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			sml::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;

			//	Get the range of each axis coordinate
			//	to scale the object to fit in our engine unit scale
			//	after parsing

			if (this->isRangeInit == false)
			{
				this->isRangeInit = true;
				this->max = vector;
				this->min = vector;
			}

			if (vector.x > this->max.x)
			{
				this->max.x = vector.x;
			}
			if (vector.y > this->max.y)
			{
				this->max.y = vector.y;
			}
			if (vector.z > this->max.z)
			{
				this->max.z = vector.z;
			}

			if (vector.x < this->min.x)
			{
				this->min.x = vector.x;
			}
			if (vector.y < this->min.y)
			{
				this->min.y = vector.y;
			}
			if (vector.z < this->min.z)
			{
				this->min.z = vector.z;
			}
			vertex.pos = vector;

			if (mesh->mNormals)
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.norm = vector;
			}
			else
			{
				vertex.norm = sml::vec3();
			}
			if (mesh->mTextureCoords[0] != NULL)
			{
				vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.texCoord = sml::vec2();
			}
			vertices.push_back(vertex);
		}
		extractBoneInfo(vertices, mesh, scene);

		//	Indices

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace	face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		/*textures.insert(textures.end(),
			std::make_move_iterator(diffuseMaps.begin()),
			std::make_move_iterator(diffuseMaps.end()));*/
		loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
		/*textures.insert(textures.end(),
			std::make_move_iterator(specularMaps.begin()),
			std::make_move_iterator(specularMaps.end()));*/

		/*MeshGeometry meshGeometry{std::move(vertices), std::move(indices)};
		//std::shared_ptr<GLMesh> glMesh = std::make_shared<GLMesh>(data, textures);
		std::shared_ptr<MeshInternal> res = std::make_shared<MeshInternal>(meshGeometry);
		//res->setName(mesh->mName.C_Str());
		return res;*/
		return {std::move(vertices), std::move(indices)};
	}

	void AssimpImporter::processNodeBones(aiNode* node, const aiScene* scene, const sml::mat4& parentMat)
	{
		sml::mat4 transform = parentMat * AssimpToSmlMatrix(node->mTransformation);
		std::string name(node->mName.data);
		std::map<std::string, BoneInfo>::iterator it =
			this->bones.find(name);
		if (it != this->bones.end())
		{
			BoneInfo& bone = it->second;
			bone.originalMatrix = transform;
			bone.modelMatrix = transform;
			bone.localMatrix = transform * bone.offsetMatrix;
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNodeBones(node->mChildren[i], scene, transform);
		}
	}

	void AssimpImporter::processNode(aiNode* node, const aiScene* scene)
	{
		//std::cout << "Reading node " << node->mName.C_Str() << std::endl;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			//std::cout << "Reading mesh " << mesh->mName.C_Str() << std::endl;
			// OPTI !! Multiple nodes can refer to the same mesh
			meshes.emplace_back(processMesh(mesh, scene));
			//meshes.back().
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void AssimpImporter::readMissingBones(const aiScene* scene)
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			aiAnimation* anim = scene->mAnimations[i];
			if (anim == nullptr)
				continue;
			for (unsigned int j = 0; j < anim->mNumChannels; j++)
			{
				aiNodeAnim* bone = anim->mChannels[i];
				std::string name = bone->mNodeName.data;
				if (this->bones.find(name) == this->bones.end())
				{
					std::cout << "Found missing bone '" << name << "'" << std::endl;
					this->bones[name].id = nbBones;
					//this->bones[name].fromParentMatrix = AssimpToSmlMatrix(bone->mTransformation)
					this->nbBones++;
				}
			}
		}
	}

	void AssimpImporter::loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, const std::string& typeName, const aiScene* scene)
	{
		//std::vector<std::shared_ptr<Texture>> textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString	str;
			mat->GetTexture(type, i, &str);
			std::filesystem::path textPath(str.C_Str());
			textPath.make_preferred();
			std::string	pathStr = directory + '/' + textPath.string();
			std::string filename = pathStr.substr(pathStr.find_last_of("/") + 1);
			const aiTexture* texture;
			//std::shared_ptr<Texture> assetTexture;
			if ((texture = scene->GetEmbeddedTexture(str.C_Str())))
			{
				/*if (texture->mHeight == 0)
				{
					assetTexture = AssetManager::getInstance().loadEmbeddedAsset<Texture>(this->path,
						filename, reinterpret_cast<unsigned char*>(texture->pcData),
						texture->mWidth, typeName);
				}
				else
				{
					assetTexture = AssetManager::getInstance().loadEmbeddedAsset<Texture>(this->path,
						filename, reinterpret_cast<unsigned char *>(texture->pcData),
						texture->mWidth * texture->mHeight, typeName);
				}*/
				aiTextures.push_back(texture);
			}
			else
			{
				//assetTexture = AssetManager::getInstance().loadAsset<Texture>(pathStr, typeName);
				texturePaths.emplace_back(pathStr, typeName);
			}
			/*if (assetTexture != nullptr)
			{
				textures.push_back(assetTexture);
			}*/

		}
		//return textures;
	}

	const std::vector<const aiTexture*>& AssimpImporter::getAiTextures()
	{
		return aiTextures;
	}

	const std::vector<std::pair<std::string, std::string>>& AssimpImporter::getTexturePaths()
	{
		return texturePaths;
	}
}