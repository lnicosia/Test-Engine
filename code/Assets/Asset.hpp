#ifndef _ASSET_HPP_
# define _ASSET_HPP_

#include <string>
#include <filesystem>
#include <vector>

namespace te
{
	class Asset
	{
	public:
		Asset();
		/**	Constructor for loading assets from files
		*/
		Asset(const std::vector<std::filesystem::path>& paths);
		/**	Constructor for creating assets from code
		*/
		Asset(const std::string& path);
		/**	The goal of assets is to keep their unicity
		*/
		Asset(const Asset& ref) = delete;
		Asset(Asset&& ref);
		/**	The goal of assets is to keep their unicity
		*/
		Asset& operator=(const Asset& ref) = delete;
		Asset& operator=(Asset&& ref);
		~Asset();

		//	Accessors

		virtual const std::string&
			getName() const;

		virtual const std::string&
			getEmbeddedName() const;

		const uint32_t&
			getId() const;

		const std::filesystem::path&
			getPath(int index = 0) const;

		const std::vector<std::filesystem::path>&
			getPaths() const;

		const bool
			isLoaded() const;

		virtual const std::string
			getAssetType() const = 0;

		//	Setters

		void
			setName(const std::string& name);

		void
			setEmbeddedName(const std::string& name);

		//	No setter for ID

		void
			setPath(const std::filesystem::path& path, int index = 0);

		void
			setPaths(const std::vector<std::filesystem::path>& paths);

	protected:

		std::string				name;

		/**	Some assets (including Shaders) may have 2 or even more paths defining them
		**	In this case all of their paths will be checked when requesting them
		*/
		std::vector<std::filesystem::path> paths;

		/**	Assets can be embedded inside a file. In this case, differenciate
		**	them with a name
		*/
		std::string	embeddedName;

		bool					loaded;

	private:

		uint32_t				id;

		static uint32_t			count;
	};

	std::ostream& operator<<(std::ostream& o, Asset const& asset);
}
#endif // _ASSET_HPP_