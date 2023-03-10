// code.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <exception>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

#include "SDL.h"
#define STB_IMAGE_IMPLEMENTATION
# include "stb_image.h"
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
float	drawRay(Point2<float> pos, float angle, int x, uint32_t color, uint32_t* pixels);

enum GameState
{
	PLAYING,
	PAUSED
};

class Texture
{
private:
	unsigned char* img;
	int w;
	int h;
	int nChannels;

public:
	Texture() = delete;
	Texture(unsigned char *img, int w, int h, int nChannels): img(img), w(w), h(h), nChannels(nChannels)
	{}

	const int getWidth() const
	{
		return w;
	}

	const int getHeight() const
	{
		return h;
	}

	const int getChannels() const
	{
		return nChannels;
	}

	const unsigned char* getImg() const
	{
		return img;
	}

	void free()
	{
		stbi_image_free(img);
	}
};

Point2<int>	winSize(800, 600);
Point2<float>	oldMouse;
float		mouseSpeed = 0.001f;
uint32_t*	pixels;
SDLEvents	events;
std::vector<std::vector<int>>	map
{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};
float	mapScale = 20;
Point2<int>	mapPos(0, 0);
Point2<int>	mapSize(200, 200);
Point2<int> mapCenter(mapPos + mapSize / 2);
int	mapMaxX = 9;
int	mapMaxY = 9;
bool running = true;
GameState gameState = PLAYING;
std::vector<Texture> textures;

float	pointDistance(const Point2<float>& p1, const Point2<float>& p2)
{
	float x = p2.x - p1.x;
	float y = p2.y - p1.y;
	return sqrt(x * x + y * y);
}

void	drawCircle(const Point2<int> pos, float radius, uint32_t color)
{
	Point2<int> p;
	float rad2;

	rad2 = radius * radius;
	for (int y = pos.y - radius; y < pos.y + radius; y++)
	{
		for (int x = pos.x - radius; x < pos.x + radius; x++)
		{
			p.x = x;
			p.y = y;
			float xDiff = pos.x - x;
			float yDiff = pos.y - y;
			if (xDiff * xDiff + yDiff * yDiff < rad2 &&
				x >= 0 && x < winSize.w && y >= 0 && y < winSize.h)
				pixels[x + y * winSize.w] = color;
		}
	}
}

void	drawCircleOnMap(const Point2<int> pos, float radius, uint32_t color)
{
	Point2<int> p;
	float rad2;

	rad2 = radius * radius;
	for (int y = pos.y - radius; y < pos.y + radius; y++)
	{
		for (int x = pos.x - radius; x < pos.x + radius; x++)
		{
			p.x = x;
			p.y = y;
			float xDiff = pos.x - x;
			float yDiff = pos.y - y;
			if (xDiff * xDiff + yDiff * yDiff < rad2 &&
				x >= mapPos.x && x < mapPos.x + mapSize.x
				&& y >= mapPos.y && y < mapPos.y + mapSize.y)
				pixels[x + y * winSize.w] = color;
		}
	}
}

/* Draw circle with inside and outline color
*/
void	drawCircle(Point2<int> pos, float radius, uint32_t insideColor, uint32_t outsideColor)
{

}

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

void	drawRectangleOnMap(Point2<int> pos, Point2<int> size, uint32_t color)
{
	Point2<int> coord;

	for (int y = 0; y < size.h; y++)
	{
		coord.x = pos.x;
		coord.y = pos.y + y;
		if (coord.x >= mapPos.x && coord.x < mapPos.x + mapSize.x
			&& coord.y >= mapPos.y && coord.y < mapPos.y + mapSize.y)
			pixels[coord.x + coord.y * winSize.w] = color;
		coord.x = pos.x + size.w;
		coord.y = pos.y + y;
		if (coord.x >= mapPos.x && coord.x < mapPos.x + mapSize.x
			&& coord.y >= mapPos.y && coord.y < mapPos.y + mapSize.y)
			pixels[coord.x + coord.y * winSize.w] = color;
	}
	for (int x = 0; x < size.w && pos.x + x < winSize.w; x++)
	{
		coord.x = pos.x + x;
		coord.y = pos.y;
		if (coord.x >= mapPos.x && coord.x < mapPos.x + mapSize.x
			&& coord.y >= mapPos.y && coord.y < mapPos.y + mapSize.y)
			pixels[coord.x + coord.y * winSize.w] = color;
		coord.x = pos.x + x;
		coord.y = pos.y + size.y;
		if (coord.x >= mapPos.x && coord.x < mapPos.x + mapSize.x
			&& coord.y >= mapPos.y && coord.y < mapPos.y + mapSize.y)
			pixels[coord.x + coord.y * winSize.w] = color;
	}
}

void	drawRectangleOnMap(Point2<int> pos, Point2<int> size, uint32_t outsideColor, uint32_t insideColor)
{
	for (int y = pos.y; y < pos.y + size.h; y++)
	{
		for (int x = pos.x; x < pos.x + size.w; x++)
		{
			if (x >= mapPos.x && x < mapPos.x + mapSize.x
				&& y >= mapPos.y && y < mapPos.y + mapSize.y)
				pixels[x + y * winSize.w] = insideColor;
		}
	}
}

void	drawLine(Point2<int> p1, Point2<int> p2, uint32_t color, uint32_t* pixels)
{
	Vector2<float> v(p1, p2);
	float x = p1.x;
	float y = p1.y;
	v.normalize();
	while ((int)x != p2.x || (int)y != p2.y)
	{
		if (x >= 0 && x < winSize.w && y >= 0 && y < winSize.h)
			pixels[(int)x + (int)y * winSize.w] = color;
		if ((int)x != p2.x)
			x += v.x;
		if ((int)y != p2.y)
			y += v.y;
	}
}

void	drawImg(Point2<int> pos, const Texture& texture, uint32_t* pixels)
{
	Point2<int> coord;
	const unsigned char* img = texture.getImg();
	for (int h = 0; h < texture.getHeight(); h++)
	{
		for (int w = 0; w < texture.getWidth(); w++)
		{
			coord.x = pos.x + w;
			coord.y = pos.y + h;
			if (coord.x >= 0 && coord.x < winSize.w && coord.y >= 0 && coord.y < winSize.h)
			{
				uint8_t R, G, B;
				int imgCoord = texture.getChannels() * (w + h * texture.getWidth());
				R = img[imgCoord];
				G = img[imgCoord + 1];
				B = img[imgCoord + 2];
				pixels[coord.x + coord.y * winSize.w] = R << 24 | G << 16 | B << 8 | 0xFF;
			}
		}
	}
}

void	drawColumnOfImg(Point2<int> start, int length, float column, bool side, const Texture& texture, uint32_t* pixels)
{
	int i = 0;
	const unsigned char* img = texture.getImg();
	int columnIndex = column * texture.getWidth();
	//printf("column index = %d\n", columnIndex);
	while (i < length)
	{
		int rowIndex = (i / (float)length) * texture.getHeight();
		if (start.y + i >= 0 && start.y + i < winSize.h)
		{
			uint8_t R, G, B;
			int imgCoord = texture.getChannels() * (int)(columnIndex + rowIndex * texture.getWidth());

			R = img[imgCoord];
			G = img[imgCoord + 1];
			B = img[imgCoord + 2];
			if (side == false)
			{
				R /= 2;
				G /= 2;
				B /= 2;
			}
			pixels[start.x + (start.y + i) * winSize.w] = R << 24 | G << 16 | B << 8 | 0xFF;
		}
		i++;
	}
}

void	drawLineOnMap(Point2<int> p1, Point2<int> p2, uint32_t color, uint32_t* pixels)
{
	Vector2<float> v(p1, p2);
	float x = p1.x;
	float y = p1.y;
	v.normalize();
	while ((int)x != p2.x || (int)y != p2.y)
	{
		if (x >= mapPos.x && x < mapPos.x + mapSize.x
			&& y >= mapPos.y && y < mapPos.y + mapSize.y)
			pixels[(int)x + (int)y * winSize.w] = color;
		if ((int)x != p2.x)
			x += v.x;
		if ((int)y != p2.y)
			y += v.y;
	}
}

struct Player
{
private:
	Vector2<float>	direction;
	Vector2<float>	rightDirection;
	Point2<float>	pos;
	float	angle;
	float	fov;
	float	topRayAngle;
	float	bottomRayAngle;
	float	speed;

public:

	Player():
		pos(5, 5),
		direction(1, 0),
		rightDirection(direction.x + cos(M_PI / 2), direction.y + sin(M_PI / 2)),
		angle(0),
		fov((float)M_PI / 2),
		topRayAngle(angle + fov / 2),
		bottomRayAngle(angle - fov / 2),
		speed(0.05f)
	{
		direction.normalize();
	}

	void	updateAngle()
	{
		this->angle += (events.mousePos.x - oldMouse.x) * mouseSpeed;
		this->direction.x = cos(this->angle);
		this->direction.y = sin(this->angle);
		this->bottomRayAngle = angle + fov / 2;
		this->topRayAngle = angle - fov / 2;
		this->rightDirection.x = cos(this->angle + M_PI / 2);
		this->rightDirection.y = sin(this->angle + M_PI / 2);
	}

	void	drawOnMap() const
	{
		Point2<int> winPos(mapCenter.x , mapCenter.y);
		Point2<int> end(winPos.x + cos(angle) * 70, winPos.y + sin(angle) * 70);
		drawLineOnMap(winPos, end, 0xFF0000FF, pixels); 
		/*end = Point2<int>(winPos.x + cos(topRayAngle) * 100, winPos.y + sin(topRayAngle) * 100);
		drawLineOnMap(winPos, end, 0xFFFFFFFF, pixels);
		end = Point2<int>(winPos.x + cos(bottomRayAngle) * 100, winPos.y + sin(bottomRayAngle) * 100);
		drawLineOnMap(winPos, end, 0xFFFFFFFF, pixels);*/
		drawCircleOnMap(winPos, mapScale / (float)8, 0xFFFFFFFF);
	}

	void	draw() const
	{
		drawOnMap();
	}

	void	drawRays() const
	{
		float ratio = fov / winSize.w;
		float currAngle = this->angle + fov / 2;
		float dist;
		bool side;
		uint32_t color;

		for (int x = 0; x < winSize.w; x++)
		{
			Point2<int> winPos(winSize.w / 2, winSize.h / 2);
			
			Point2<int> end(winPos.x + cos(currAngle) * 100, winPos.y + sin(currAngle) * 100);
			dist = drawRay(pos, currAngle, x, 0xFFFF00FF, pixels);
			//printf("Dist[%d] = %f\n", x, dist);
			//printf("angle = %f\n", (this->angle - currAngle) / (float)M_PI * 180);
			//if (cos(dist) == )
			currAngle -= ratio;
		}
	}

	void	forward()
	{
		this->pos.x += this->direction.x * this->speed;
		this->pos.y += this->direction.y * this->speed;
	}

	void	backward()
	{
		this->pos.x -= this->direction.x * this->speed;
		this->pos.y -= this->direction.y * this->speed;
	}

	void	left()
	{
		this->pos.x -= this->rightDirection.x * this->speed;
		this->pos.y -= this->rightDirection.y * this->speed;
	}

	void	right()
	{
		this->pos.x += this->rightDirection.x * this->speed;
		this->pos.y += this->rightDirection.y * this->speed;
	}
	const Point2<float>& getPos() const
	{
		return pos;
	}

	const Vector2<float>& getDirection() const
	{
		return direction;
	}

	const float& getAngle() const
	{
		return angle;
	}
};

Player	player;

float	drawRay(Point2<float> pos, float angle, int x, uint32_t color, uint32_t* pixels)
{
	Vector2<float> v(cos(angle), sin(angle));
	float mapX = pos.x;
	float mapY = pos.y;
	float diffX;
	float diffY;
	float newMapX = mapX, newMapY = mapY;
	Point2<int> screen(mapCenter);
	Point2<int> end;
	bool hit = false;
	float dist;
	v.normalize();
	//printf("Ray starting from [%f %f] with angle = %f\n", pos.x, pos.y, angle / M_PI * 180);
	//printf("Vector = [%f %f]\n", v.x, v.y);
	int count = 0;
	int textIndex;
	while (hit == false && ((int)mapX < mapMaxX && (int)mapY < mapMaxY
		&& (int)mapX >= 1 && (int)mapY >= 1))
	{
		//printf("Screen pos = [%d %d]\n", screenX, screenY);
		//if (screenX >= 0 && screenX < winSize.w && screenY >= 0 && screenY < winSize.h)
		//	pixels[screenX + screenY * winSize.w] = color;
		//printf("ceil(%f) = %f\n", 5.0f, ceil(5.0f));
		float nextX;
		if (v.x > 0)
			nextX = floor(mapX) + 1;
		else
			nextX = ceil(mapX) - 1;
		float nextY;
		if (v.y > 0)
			nextY = floor(mapY) + 1;
		else
			nextY = ceil(mapY) - 1;
		diffX = (nextX - mapX) / v.x;
		diffY = (nextY - mapY) / v.y;
		if (diffX < diffY)
		{
			newMapX += v.x * diffX;
			newMapY += v.y * diffX;
		}
		else
		{
			newMapX += v.x * diffY;
			newMapY += v.y * diffY;
		}
		//printf("Map pos = [%f %f]\n", newMapX, newMapY);
		end.x = (int)(screen.x + (newMapX - mapX) * mapScale);
		end.y = (int)(screen.y + (newMapY - mapY) * mapScale);
		mapX = newMapX;
		mapY = newMapY;

		drawLineOnMap(screen, end, color, pixels);

		Point2<int> coord(mapX, mapY);
		//printf("Coord = [%d %d]\n", coord.x, coord.y);
		if (diffX < diffY && v.x < 0)
			coord.x = (int)(ceil(mapX) - 1);
		if (diffY < diffX && v.y < 0)
			coord.y = (int)(ceil(mapY) - 1);
		//printf("Coord = [%d %d] (%d)\n", coord.x, coord.y, map[coord.y][coord.x]);
		if (map[coord.y][coord.x] == 0)
		{
			//drawCircle(end, 4, 0xFFFFFFFF);
		}
		else
		{
			//drawCircleOnMap(end, mapScale / (float)10, 0x00FF00FF);
			hit = true;
			textIndex = map[coord.y][coord.x] - 1;
		}
		//drawCircle(end, 4, 0x00FF00FF);
		screen = end;
		count++;
		//if (count == 2)
		//break;
	}
	if (hit == false)
		return 0.0f;
	///printf("Intersects at map[%f %f]\n", mapX, mapY);
	// TODO Could be replaced by cos/sin formula (cheaper)
	dist = pointDistance(pos, Point2<float>(mapX, mapY));
	
	float distCorrec = dist;// *cos(player.getAngle() - angle);
	int size;
	if (distCorrec < 400 / winSize.h)
		size = winSize.h / 2;
	else
		size = (int)(200 / distCorrec);
	float column;
	float dummy;
	bool side;
	if (diffX < diffY)
	{
		color = 0x000088FF;
		column = modf(mapY, &dummy);
		side = true;
	}
	else
	{
		color = 0x0000FFFF;
		column = modf(mapX, &dummy);
		side = false;
	}
	//printf("column = %f\n", column);
	/*drawLine(Point2<int>(winSize.w - 1 - x, winSize.h / 2 - size),
		Point2<int>(winSize.w - 1 - x, winSize.h / 2 + size),
		color, pixels);*/
	
	drawColumnOfImg(Point2<int>(winSize.w - 1 - x, winSize.h / 2 - size), size * 2, column,
		side, textures[textIndex], pixels);
	return pointDistance(pos, Point2<float>(mapX, mapY));
}

void	drawMap(const std::vector<std::vector<int>>& map)
{
	Point2<int> size(mapScale, mapScale);
	Point2<int> pos;
	Point2<int> startPos(mapCenter.x  - player.getPos().x * size.x,
		mapCenter.y  - player.getPos().y * size.y);
	Point2<float> startPos2(player.getPos().x - map.size(), player.getPos().y - map.size());
	pos.y = startPos.y;
	for (size_t y = 0; y < map.size(); y++)
	{
		pos.x = startPos.x;
		for (size_t x = 0; x < map[y].size(); x++)
		{
			if (map[y][x] == 1)
			{
				drawRectangleOnMap(pos, size, 0, 0xFF0000FF);
				drawRectangleOnMap(pos, size, 0xFFFFFFFF);
			}
			else
				drawRectangleOnMap(pos, size, 0xFFFFFFFF);
			pos.x += size.x;
		}
		pos.y += size.y;
	}
	drawRectangle(mapPos, mapSize, 0xFFFFFFFF);
}

void	clearImg()
{
	for (int y = 0; y < winSize.h / 2; y++)
	{
		for (int x = 0; x < winSize.w; x++)
		{
			pixels[x + y * (int)winSize.w] = 0x253342FF;
		}
	}
	for (int y = winSize.h / 2; y < winSize.h; y++)
	{
		for (int x = 0; x < winSize.w; x++)
		{
			pixels[x + y * (int)winSize.w] = 0x5C62D6FF;
		}
	}
}

void Forward()
{
	player.forward();
}

void Backward()
{
	player.backward();
}

void Right()
{
	player.right();
}

void Left()
{
	player.left();
}

void ChangeGameState()
{
	if (gameState == PLAYING)
	{
		gameState = PAUSED;
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	else
	{
		gameState = PLAYING;
		SDL_SetRelativeMouseMode(SDL_TRUE);
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


	Binding forward("forward", SDLK_UP, SDLK_w, true);
	forward.whenPressed = std::shared_ptr<ActionWrapper>(new Action<>(std::function<void()>(Forward)));
	events.bindings.push_back(forward);

	Binding backward("backward", SDLK_DOWN, SDLK_s, true);
	backward.whenPressed = std::shared_ptr<ActionWrapper>(new Action<>(std::function<void()>(Backward)));
	events.bindings.push_back(backward);

	Binding left("left", SDLK_LEFT, SDLK_a, true);
	left.whenPressed = std::shared_ptr<ActionWrapper>(new Action<>(std::function<void()>(Left)));
	events.bindings.push_back(left);

	Binding right("right", SDLK_RIGHT, SDLK_d, true);
	right.whenPressed = std::shared_ptr<ActionWrapper>(new Action<>(std::function<void()>(Right)));
	events.bindings.push_back(right);

	MouseBinding leftB("Mouse left", SDL_BUTTON_LEFT, 0, false);
	leftB.onRelease = std::shared_ptr<ActionWrapper>(new Action<>(std::function<void()>(ChangeGameState)));
	events.mouseBindings.push_back(leftB);

	Binding updateRotate("updateRotate", SDLK_ESCAPE, 0, true);
	updateRotate.onRelease = leftB.onRelease;
	events.bindings.push_back(updateRotate);

	stbi_set_flip_vertically_on_load(true);

	Point2<int> imgSize;
	int nChannels;
#ifdef __unix__
	std::string path = "../../../resources/textures/bigdoor2.bmp";
#else
	std::string path = "../../../../resources/textures/bigdoor2.bmp";
#endif
	unsigned char* img = stbi_load(path.c_str(),
		&imgSize.x, &imgSize.y, &nChannels, 0);
	if (!img)
	{
		std::cerr << std::endl << "Failed to load texture '" + path << " '" << std::endl;
		std::cerr << stbi_failure_reason() << std::endl;
		stbi_image_free(img);
		return -1;
	}
	Texture currTexture(img, imgSize.x, imgSize.y, nChannels);
	textures.push_back(currTexture);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	while (running)
	{

		if (events.handle() == 1)
			running = false;

		clearImg();

		drawMap(map);

		if (gameState == PLAYING)
			player.updateAngle();
		player.draw();
		player.drawRays();

		oldMouse = events.mousePos;

		SDL_UpdateTexture(texture, nullptr, pixels, sizeof(Uint32) * winSize.x);
		SDL_RenderTexture(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete[] pixels;
	for (auto t : textures)
		t.free();
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
