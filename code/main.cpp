// code.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "SDL.h"

void SetupGLOptions()
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

int main()
{
	std::cout << "Hello CMake." << std::endl;
	if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0) {
		throw std::runtime_error( "Couldn't initialize SDL : \n" + std::string(SDL_GetError()) );
	}
	SetupGLOptions();
	SDL_Window *window = SDL_CreateWindow("Yop", 800, 600, 0);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	SDL_Event events;
	bool running = true;
	while (running)
	{
		SDL_PollEvent(&events);
		switch (events.type)
		{
		case SDL_EVENT_QUIT:
			running = false;
			break;
		}
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return 0;
}
