#ifndef _ASSET_MANAGER_HPP_
# define _ASSET_MANAGER_HPP_

#include "Asset.hpp"
#include "Debug/Log.hpp"
#include "Textures/Texture.hpp"
#include "CheckFileType.hpp"

#include <map>
#include <iostream>
#include <memory>

namespace te
{
	class AssetManager
	{
	public:
		AssetManager(const AssetManager& ref) = delete;
		AssetManager(const AssetManager&& ref) = delete;
		AssetManager&	operator=(const AssetManager& ref) = delete;
		AssetManager&	operator=(const AssetManager&& ref) = delete;
		~AssetManager();

		static AssetManager&
			getInstance();

		/**	For a given type, path and args, search the content of the manager
		**	for an existing and corresponding asset. If found, return it.
		**	If not, load it with its own load function
		**	TODO: Add a requires(Asset) or smt
		*/
		template <typename T, typename ... Args>
		std::shared_ptr<T> loadAsset(const std::string& path, Args... args)
		{
			TE_LOG(TE_RESOURCE_LOG, TE_DISPLAY, "Loading '%s'\n", path.c_str());
			if (!IsReg(path))
			{
				return std::shared_ptr<T>();
			}
			for (const auto& pair : this->assets)
			{
				std::shared_ptr<Asset> asset = pair.second;
				if (std::filesystem::exists(asset->getPath())
					&& std::filesystem::equivalent(asset->getPath(),
						std::filesystem::path(path)))
				{
					std::shared_ptr<T> tmp = std::dynamic_pointer_cast<T>(this->assets[asset->getId()]);
					//	If the cast successed, we found our asset. Return it instead of
					//	loading it again
					if (tmp)
					{
						return tmp;
					}
					//	If not, it can be an asset with the same path but of different type.
					//	For example, embedded textures or animations share the same path
					//	with their object
				}
			}

			std::shared_ptr<T> ptr = std::make_shared<T>(path, std::forward<Args>(args)...);
			if (ptr->isLoaded() == false)
			{
				return std::shared_ptr<T>();
			}
			assets.emplace(std::make_pair(ptr->getId(), ptr));
			return ptr;
		}

		/**	For a given type, path and args, search the content of the manager
		**	for an existing and corresponding asset. If found, return it.
		**	If not, load it with its own load function
		*/
		template <typename T, typename ... Args>
		std::shared_ptr<T> loadEmbeddedAsset(const std::string& path, std::string& embeddedName, Args... args)
		{
			if (!IsReg(path))
			{
				return std::shared_ptr<T>();
			}
			for (const auto& pair : this->assets)
			{
				std::shared_ptr<Asset> asset = pair.second;
				if (std::filesystem::exists(asset->getPath())
					&& std::filesystem::equivalent(asset->getPath(),
						std::filesystem::path(path)))
				{
					std::shared_ptr<T> tmp = dynamic_pointer_cast<T>(this->assets[asset->getId()]);
					//	If the cast successed, we found our asset. Return it instead of
					//	loading it again
					if (tmp && tmp->getEmbeddedName() == embeddedName)
					{
						return tmp;
					}
					//	If not, it can be an asset with the same path but of different type.
					//	For example, embedded textures or animations share the same path
					//	with their object
				}
			}

			std::shared_ptr<T> ptr = std::make_shared<T>(path, std::forward<Args>(args)...);
			if (ptr->isLoaded() == false)
			{
				return std::shared_ptr<T>();
			}
			ptr->setEmbeddedName(embeddedName);
			assets.emplace(std::make_pair(ptr->getId(), ptr));
			return ptr;
		}

		/**	Same as loadAsset but does not load if not found.
		**	Use this function when you know for sure that the asset is already loaded
		**	Paths are not unique, so the first asset found with this path
		**	will be returned
		*/
		template <typename T>
		std::shared_ptr<T> getAsset(const std::string& path)
		{
			for (const auto& pair : this->assets)
			{
				std::shared_ptr<Asset> asset = pair.second;
				std::filesystem::path stdPath(path);
				if (std::filesystem::exists(stdPath)
					&& std::filesystem::exists(asset->getPath())
					&& std::filesystem::equivalent(std::filesystem::path(asset->getPath()),
						std::filesystem::path(path)))
				{
					std::shared_ptr<T> tmp = dynamic_pointer_cast<T>(this->assets[asset->getId()]);
					if (tmp)
					{
						return tmp;
					}
				}
			}
			return std::shared_ptr<T>();
		}

		/**	Retrieve an asset by its ID if found or nullptr.
		**	IDs are unique
		*/
		template <typename T>
		std::shared_ptr<T> getAsset(uint32_t id)
		{
			std::map<uint32_t, std::shared_ptr<Asset>>::iterator it =
				this->assets.find(id);
			if (it == this->assets.end())
			{
				return std::shared_ptr<T>();
			}
			std::shared_ptr<T> tmp = dynamic_pointer_cast<T>(it->second);
			if (tmp)
			{
				return tmp;
			}
			return std::shared_ptr<Asset>();
		}

		/**	Retrieve an asset by its name if found or nullptr.
		**	Names are not unique, so the first asset found with this name
		**	will be returned
		*/
		template <typename T>
		std::shared_ptr<T> getAssetByName(const std::string& name)
		{
			for (const auto& pair : this->assets)
			{
				std::shared_ptr<Asset> asset = pair.second;
				if (asset->getName() == name)
				{
					std::shared_ptr<T> tmp = dynamic_pointer_cast<T>(this->assets[asset->getId()]);
					if (tmp)
					{
						return tmp;
					}
				}
			}
			return std::shared_ptr<T>();
		}

		template <typename T>
		void addAsset(std::shared_ptr<T> asset)
		{
			if (this->assets.find(asset->getId()) == this->assets.end())
				assets.emplace(std::make_pair(asset->getId(), asset));
		}

		template <typename T>
		std::vector<std::shared_ptr<T>> getAssetsOfType()
		{
			std::vector<std::shared_ptr<T>> res;
			for (const auto& pair: this->assets)
			{
				std::shared_ptr<T> asset = dynamic_pointer_cast<T>(this->assets[pair.second->getId()]);
				if (asset)
				{
					res.push_back(asset);
				}
			}
			return res;
		}

		template <typename T, typename ... Args>
		std::shared_ptr<Texture> loadDefaultTexture(const std::string& path, Args... args)
		{
			defaultTexture = loadAsset<T>(path, args...);
			return defaultTexture;
		}

		std::shared_ptr<Texture> getDefaultTexture()
		{
			return defaultTexture;
		}

		void
			printContent(void) const;

		/**	Clear contents
		*	It would be nice to change this because we have to manually clear
		*	to make sure that assets are released before their context is destroyed
		*/
		void
			clear(void);

	private:
		AssetManager();

		std::map<uint32_t, std::shared_ptr<Asset>>	assets;
		std::shared_ptr<Texture> defaultTexture{};
	};
}
#endif // _ASSET_MANAGER_HPP_