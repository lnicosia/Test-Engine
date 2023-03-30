#ifndef _WINDOW_HPP_
# define _WINDOW_HPP_

#include "Maths/math.hpp"
#include "RendererType.hpp"
#include "Inputs/Events.hpp"

namespace te
{
	class Window
	{
	public:
		Window() = delete;
		Window(RendererType rType);
		Window(int w, int h, RendererType rType);
		virtual ~Window();
		const int getWidth() const;
		const int getHeight() const;

		int	handleEvents();
		std::shared_ptr<Events> events;

		virtual int	loadVulkan() = 0;
		virtual int getVulkanInstanceExtensions(uint32_t* count, std::vector<const char*>& names) = 0;
		virtual int loadOpenGL() = 0;

	private:
		bool	resizable;
		double	ratio;
		RendererType rType;

	protected:
		int w = 1600;
		int h = 900;
		
	};


}

#endif // _WINDOW_HPP_