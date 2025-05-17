#include "Asset.hpp"

#include <iostream>
#include <utility>

namespace te
{
	Asset::Asset(const std::string& path): paths({std::filesystem::path(path)}),
		name(""), embeddedName(""), loaded(true)
	{
		id = count;
		count++;
		size_t size = this->paths.size();
		for (size_t i = 0; i < size; i++)
		{
			if (i != 0)
			{
				name += "-";
			}
			name += this->paths[i].filename().string();
		}
	}

	Asset::Asset(): paths({""}), name(),  embeddedName(""), loaded(true)
	{
		id = count;
		count++;
	}

	Asset::Asset(Asset&& ref):
		name(std::move(ref.name)),
		embeddedName(std::move(ref.embeddedName)),
		paths(std::move(ref.paths)),
		id(std::exchange(ref.id, 0)),
		loaded(std::exchange(ref.loaded, false))
	{

	}

	Asset&	Asset::operator=(Asset&& ref)
	{
		this->name = std::move(ref.name);
		this->embeddedName = std::move(ref.embeddedName);
		this->paths = std::move(ref.paths);
		this->id = std::exchange(ref.id, 0);
		this->loaded = std::exchange(ref.loaded, false);
		return *this;
	}

	Asset::~Asset()
	{

	}

	//	Accessors

	const std::string& Asset::getName() const
	{
		return name;
	}

	const std::string& Asset::getEmbeddedName() const
	{
		return embeddedName;
	}

	const uint64_t& Asset::getId() const
	{
		return id;
	}

	const std::filesystem::path& Asset::getPath(int index) const
	{
		if (index >= paths.size())
		{
			std::cerr << "Error: requesting path #" << index << " of array of size ";
			std::cerr << paths.size() << std::endl;
		}
		return paths[index];
	}

	const std::vector<std::filesystem::path>& Asset::getPaths() const
	{
		return paths;
	}

	const bool	Asset::isLoaded() const
	{
		return loaded;
	}

	//	Setters

	void	Asset::setName(const std::string& name)
	{
		this->name = name;
	}

	void	Asset::setEmbeddedName(const std::string& name)
	{
		this->embeddedName = name;
	}

	void	Asset::setPath(const std::filesystem::path& path, int index)
	{
		if (index >= paths.size())
		{
			std::cerr << "Error: setting path #" << index << " of array of size ";
			std::cerr << paths.size() << std::endl;
		}
		this->paths[index] = path;
	}

	void	Asset::setPaths(const std::vector<std::filesystem::path>& paths)
	{
		this->paths = paths;
	}

	std::ostream& operator<<(std::ostream& o, Asset const& asset)
	{
		std::cout << "- ";
		if (asset.getEmbeddedName() != "")
		{
			std::cout << "Embedded ";
		}
		std::cout << asset.getAssetType() << " \"" << asset.getName();
		std::cout << "\"";
		if (asset.getEmbeddedName() != "")
		{
			std::cout << " (" << asset.getEmbeddedName() << ")";
		}
		std::cout << ": ID = " << asset.getId() << ", path = ";
		std::cout << asset.getPath();
		/*if (asset.isLoaded())
		std::cout << ", is loaded";
		else
		std::cout << ", is not loaded";*/
		std::cout << std::endl;
		return o;
	}

	uint32_t	Asset::count = 0;
}