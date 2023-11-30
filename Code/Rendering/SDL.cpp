#include "SDL.hpp"
#include "Debug/Log.hpp"
#include "Debug/Exception.hpp"

#include "SDL.h"
#include "SDL_ttf.h"

#include <string>
#include <stdexcept>

namespace te
{
	void SDL::Init( void )
	{
		if (initialized)
			return;

		LOG(TE_RENDERING_LOG, TE_LOG, "Initializing SDL\n");

		if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
			ThrowException( "Couldn't initialize SDL Video subsystem : " + std::string(SDL_GetError()) + "\n" );

		// No need to init EVENTS manually, it is implied in VIDEO

		//if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
		//	LOG(TE_RENDERING_LOG, TE_ERROR, "Couldn't initialize SDL Audio subsystem : %s\n", SDL_GetError());

		//if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0)
		//	LOG(TE_RENDERING_LOG, TE_ERROR, "Couldn't initialize SDL Joystick subsystem : %s\n", SDL_GetError());

		//if (SDL_InitSubSystem(SDL_INIT_HAPTIC) != 0)
		//	LOG(TE_RENDERING_LOG, TE_ERROR, "Couldn't initialize SDL Haptic subsystem : %s\n", SDL_GetError());

		//if (SDL_InitSubSystem(SDL_INIT_GAMEPAD) != 0)
		//	LOG(TE_RENDERING_LOG, TE_ERROR, "Couldn't initialize SDL Gamepad subsystem : %s\n", SDL_GetError());

		LOG(TE_RENDERING_LOG, TE_LOG, "Initializing SDL_ttf\n");
		if (TTF_Init() == -1)
			ThrowException( "Couldn't initialize SDL_ttf : \n" + std::string(TTF_GetError()) + "\n" );

		if (std::atexit(Quit) != 0)
		{
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL TTF\n");
			TTF_Quit();
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL subsystems\n");
			SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL\n");
			SDL_Quit();
			ThrowException( "Failed to setup SDL cleanup function\n" );
		}

		initialized = true;
	}

	void SDL::SetupGLOptions()
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	void SDL::Quit( void )
	{
		if (initialized)
		{
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL TTF\n");
			TTF_Quit();
			/* LEAK ?? */
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL subsystems\n");
			SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL\n");
			SDL_Quit();
			initialized = false;
		}
	}

	bool SDL::initialized = false;
}