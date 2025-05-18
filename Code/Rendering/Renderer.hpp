#ifndef _RENDERER_HPP_
# define _RENDERER_HPP_

#include "Window/Window.hpp"
#include "DebugLevel.hpp"
#include "Assets/Fonts/Font.hpp"
#include "RendererType.hpp"
#include "Rendering/GPUDevice.hpp"
#include "Window/WindowAPI.hpp"
#include "Scene/Scene.hpp"
#include "Misc/TimerManager.hpp"
#include "Camera.hpp"

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
		virtual std::shared_ptr<Texture> loadTexture(const std::string& path) = 0;

		std::shared_ptr<Window> getWindow() const;
		std::shared_ptr<GPUDevice> getDevice() const;
		RendererType getType() const;
		WindowAPI getWindowAPI() const;
		const Scene& getScene() const;
		float getDeltaTime() const;

		void addBinding(const Binding& binding);
		void addMouseBinding(const MouseBinding& binding);

	private:

		void updateWindowTitle();

	public:

		Scene scene{};
		Camera camera{};
	
		DebugLevel debugLevel = DebugLevel::TE_SHOW_FPS;

		bool running = true;

	protected:

		RendererType rType{};
		WindowAPI windowAPI{};
		TimerManager timerManager{};
		
		std::shared_ptr<Window> window{};
		std::shared_ptr<Font> uiFont{};
		std::shared_ptr<GPUDevice> device{};

	};

}

#endif // _RENDERER_HPP_