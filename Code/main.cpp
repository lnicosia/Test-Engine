
#include <iostream>
#include <exception>
#include <algorithm>
#include <chrono>
#define _USE_MATH_DEFINES
#include <math.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "Inputs/SDLEvents.hpp"
#include "Rendering/SoftwareRenderer.hpp"
#include "Rendering/VulkanRenderer.hpp"
#include "Rendering/GLRenderer.hpp"
#include "Rendering/D3DRenderer.hpp"
#include "Rendering/SoftwareRendering/Raycaster.hpp"
#include "Rendering/WindowManager.hpp"

#include "stb_image.h"


int main()
{
	/*try
	{
		te::SoftwareRenderer renderer;
		te::Raycaster	raycaster(&renderer);
		raycaster.render();
	}*/
	try
	{
		te::VulkanRenderer renderer;
		renderer.render();
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}