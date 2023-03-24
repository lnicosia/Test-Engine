#include "Texture.hpp"

namespace te
{
	class SoftwareTexture: public Texture
	{
	public:
		SoftwareTexture() = delete;
		SoftwareTexture(const std::string& path);
		SoftwareTexture(const SoftwareTexture& ref) = delete;
		SoftwareTexture(SoftwareTexture&& ref);
		SoftwareTexture& operator=(const SoftwareTexture& ref) = delete;
		SoftwareTexture& operator=(SoftwareTexture&& ref);
		~SoftwareTexture();

		const unsigned char* getImg() const;

		const std::string getAssetType() const override;

	private:
		unsigned char* img;
	};
}