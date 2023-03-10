// code.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <exception>
#define _USE_MATH_DEFINES
#include <math.h>

#include "SDL.h"
#include "Inputs/SDLEvents.class.hpp"

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

void	drawLine(Point2<int> p1, Point2<int> p2, uint32_t color, uint32_t* pixels);

Point2<int>	winSize(800, 600);
Point2<float>	oldMouse;
float		mouseSpeed = 0.001;
uint32_t*	pixels;
SDLEvents	events;
std::vector<std::vector<int>>	map
{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

struct Player
{
private:
	Vector2<float>	direction;
	float	angle;
	float	fov;
	float	topRayAngle;
	float	bottomRayAngle;

public:
	Point2<float>	pos;

	Player():
		pos(5, 5),
		direction(1, 0),
		angle(0),
		fov((float)M_PI / 2),
		topRayAngle(angle + fov / 2),
		bottomRayAngle(angle - fov / 2)
	{
	}

	void	updateAngle()
	{
		this->angle += (events.mouseGlobalPos.x - oldMouse.x) * mouseSpeed;
		this->direction.x = cos(this->angle);
		this->direction.y = sin(this->angle);
		this->topRayAngle = angle + fov / 2;
		this->bottomRayAngle = angle - fov / 2;
	}

	void	draw()
	{
		Point2<int> winPos(winSize.w / 2, winSize.h / 2);
		Point2<int> end(winPos.x + cos(angle) * 70, winPos.y + sin(angle) * 70);
		drawLine(winPos, end, 0xFF0000FF, pixels); 
		end = Point2<int>(winPos.x + cos(topRayAngle) * 100, winPos.y + sin(topRayAngle) * 100);
		drawLine(winPos, end, 0xFFFFFFFF, pixels);
		end = Point2<int>(winPos.x + cos(bottomRayAngle) * 100, winPos.y + sin(bottomRayAngle) * 100);
		drawLine(winPos, end, 0xFFFFFFFF, pixels);
	}
};

Player	player;

void	drawRectangle(Point2<int> pos, Point2<int> size, uint32_t color)
{
	Point2<int> coord;

	for (int y = 0; y < size.h; y++)
	{
		coord.x = pos.x;
		coord.y = pos.y + y;
		if (coord.x >= 0 && coord.x < winSize.w && coord.y >= 0 && coord.y < winSize.h)
			pixels[coord.x + coord.y * winSize.w] = color;
		coord.x = pos.x + size.w;
		coord.y = pos.y + y;
		if (coord.x >= 0 && coord.x < winSize.w && coord.y >= 0 && coord.y < winSize.h)
			pixels[coord.x + coord.y * winSize.w] = color;
	}
	for (int x = 0; x < size.w && pos.x + x < winSize.w; x++)
	{
		coord.x = pos.x + x;
		coord.y = pos.y;
		if (coord.x >= 0 && coord.x < winSize.w && coord.y >= 0 && coord.y < winSize.h)
			pixels[coord.x + coord.y * winSize.w] = color;
		coord.x = pos.x + x;
		coord.y = pos.y + size.y;
		if (coord.x >= 0 && coord.x < winSize.w && coord.y >= 0 && coord.y < winSize.h)
			pixels[coord.x + coord.y * winSize.w] = color;
	}
}

void	drawRectangle(Point2<int> pos, Point2<int> size, uint32_t outsideColor, uint32_t insideColor)
{
	for (int y = pos.y; y < pos.y + size.h; y++)
	{
		for (int x = pos.x; x < pos.x + size.w; x++)
		{
			if (x >= 0 && x < winSize.w && y >= 0 && y < winSize.h)
				pixels[x + y * winSize.w] = insideColor;
		}
	}
}

void	drawMap(const std::vector<std::vector<int>>& map)
{
	Point2<int> size(50, 50);
	Point2<int> pos;
	Point2<int> startPos(winSize.w / 2  - player.pos.x * size.x,
		winSize.h / 2  - player.pos.y * size.y);
	Point2<float> startPos2(player.pos.x - (map.size()), (player.pos.y - map.size()));
	pos.y = startPos.y;
	for (size_t y = 0; y < map.size(); y++)
	{
		pos.x = startPos.x;
		for (size_t x = 0; x < map[y].size(); x++)
		{
			if (map[y][x] == 1)
			{
				drawRectangle(pos, size, 0, 0xFF0000FF);
				drawRectangle(pos, size, 0xFFFFFFFF);
			}
			else
				drawRectangle(pos, size, 0xFFFFFFFF);
			pos.x += size.x;
		}
		pos.y += size.y;
	}
}

void	drawLine(Point2<int> p1, Point2<int> p2, uint32_t color, uint32_t* pixels)
{
	if (p1.x > p2.x)
		std::swap(p1, p2);
	float ratio = (p2.y - p1.y) / (float)(p2.x - p1.x);
	float y = (float)p1.y;
	while (p1.x < p2.x && p1.x < winSize.w)
	{
		if ((int)y < winSize.h)
			pixels[p1.x + (int)y * winSize.w] = color;
		p1.x++;
		y += ratio;
	}
}

void	clearImg()
{
	for (int y = 0; y < winSize.h; y++)
	{
		for (int x = 0; x < winSize.w; x++)
		{
			pixels[x + y * (int)winSize.w] = 0;
		}
	}
}

int main()
{
	std::cout << "Hello CMake." << std::endl;
	if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0) {
		throw std::runtime_error( "Couldn't initialize SDL : \n" + std::string(SDL_GetError()) );
	}
	SetupGLOptions();
	SDL_Window *window = SDL_CreateWindow("Yop", winSize.w, winSize.h, 0);
	if (!window)
		throw std::runtime_error("Could not create window");

	SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_SOFTWARE);
	if (!renderer)
		throw std::runtime_error("Could not create rendered");

	pixels = new uint32_t[winSize.w * winSize.h];
	
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING, winSize.w, winSize.h);
	if (!texture)
		throw std::runtime_error("Could not create texture");


	bool running = true;
	while (running)
	{

		if (events.handle() == 1)
			running = false;

		clearImg();

		player.updateAngle();
		player.draw();

		drawMap(map);

		oldMouse = events.mouseGlobalPos;

		SDL_UpdateTexture(texture, nullptr, pixels, sizeof(Uint32) * winSize.x);
		SDL_RenderTexture(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
