#ifndef _RENDERER_HPP_
# define _RENDERER_HPP_

#include "Window/Window.hpp"
#include "DebugLevel.hpp"
#include "Assets/Fonts/Font.hpp"
#include "RendererType.hpp"
#include "Rendering/GPUDevice.hpp"
#include "Window/WindowAPI.hpp"
#include "Scene/Scene.hpp"

#define DEFAULT_TEXTURE_PATH "Resources/Textures/bigdoor3.bmp"

namespace te
{
	class Renderer
	{
	public:
		explicit Renderer(RendererType rType = RendererType::TE_VULKAN, WindowAPI windowAPI = WindowAPI::TE_SDL);
		Renderer() = delete;
		virtual ~Renderer();

		virtual void render();
		virtual void renderText(const char* text, std::shared_ptr<Font> font,
			Point2<int> pos, int size) = 0;
		virtual std::shared_ptr<MeshInternal> createSolidMesh(const MeshGeometry& geometry) = 0;
		virtual std::shared_ptr<MeshInternal> loadMesh(const std::string& path) = 0;

		std::shared_ptr<Window> getWindow() const;
		std::shared_ptr<GPUDevice> getDevice() const;
		const RendererType getType() const;
		const WindowAPI getWindowAPI() const;
		const Scene& getScene() const;


	public:

		Scene scene{};
	
		DebugLevel	debugLevel;

		bool running;

	protected:

		RendererType rType{};
		WindowAPI windowAPI{};
		
		std::shared_ptr<Window> window{};
		std::shared_ptr<Font> uiFont{};
		std::shared_ptr<GPUDevice> device{};

	};

}

#endif // _RENDERER_HPP_