#include "Raycaster.hpp"
#include "DrawUtils.hpp"
#include "Assets/AssetManager.hpp"

namespace te
{
	void ChangeGameState(GameState& gameState)
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

	void Forward(Camera& camera)
	{
		camera.moveForward();
	}

	void Backward(Camera& camera)
	{
		camera.moveBackward();
	}

	void Right(Camera& camera)
	{
		camera.moveRight();
	}

	void Left(Camera& camera)
	{
		camera.moveLeft();
	}

	Raycaster::Raycaster(SoftwareRenderer* renderer): renderer(renderer), running(true),
		minimapPos(),
		minimapSize(Point2<int>(200, 200)),
		minimapCenter(Point2<int>(minimapPos + minimapSize / 2.0)),
		mapMaxX(24), mapMaxY(24), mapScale(20.0),
		gameState(PLAYING)
	{
#ifdef __unix__
		std::string basePath = "../../../resources/";
#else
		std::string basePath = "../../../../resources/";
#endif

		std::string texturesBasePath = basePath + "textures/wolfenstein/";

		AssetManager& assetManager = AssetManager::getInstance();

		textures.push_back(assetManager.loadAsset<SoftwareTexture>(texturesBasePath + "bluestone.png"));
		textures.push_back(assetManager.loadAsset<SoftwareTexture>(texturesBasePath + "colorstone.png"));
		textures.push_back(assetManager.loadAsset<SoftwareTexture>(texturesBasePath + "eagle.png"));
		textures.push_back(assetManager.loadAsset<SoftwareTexture>(texturesBasePath + "greystone.png"));
		textures.push_back(assetManager.loadAsset<SoftwareTexture>(texturesBasePath + "mossy.png"));
		textures.push_back(assetManager.loadAsset<SoftwareTexture>(texturesBasePath + "purplestone.png"));
		textures.push_back(assetManager.loadAsset<SoftwareTexture>(texturesBasePath + "redbrick.png"));
		textures.push_back(assetManager.loadAsset<SoftwareTexture>(texturesBasePath + "wood.png"));

		SDL_SetRelativeMouseMode(SDL_TRUE);

		map = {
			{8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
			{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
			{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
			{8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
			{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
			{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
			{8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
			{7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
			{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
			{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
			{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
			{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
			{7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
			{2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
			{2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
			{2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
			{1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
			{2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
			{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
			{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
			{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
			{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
			{2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
		};

		camera.pos = { 5, 5 };

		Binding forward("forward", SDLK_UP, SDLK_w, true);
		std::function<void(Camera&)> func = Forward;
		forward.whenPressed = std::shared_ptr<ActionWrapper>(new Action<void, Camera&>(func, camera));
		renderer->getWindow()->events->bindings.push_back(forward);

		Binding backward("backward", SDLK_DOWN, SDLK_s, true);
		func = Backward;
		backward.whenPressed = std::shared_ptr<ActionWrapper>(new Action<void, Camera&>(func, camera));
		renderer->getWindow()->events->bindings.push_back(backward);

		Binding left("left", SDLK_LEFT, SDLK_a, true);
		func = Left;
		left.whenPressed = std::shared_ptr<ActionWrapper>(new Action<void, Camera&>(func, camera));
		renderer->getWindow()->events->bindings.push_back(left);

		Binding right("right", SDLK_RIGHT, SDLK_d, true);
		func = Right;
		right.whenPressed = std::shared_ptr<ActionWrapper>(new Action<void, Camera&>(func, camera));
		renderer->getWindow()->events->bindings.push_back(right);

		MouseBinding leftB("Mouse left", SDL_BUTTON_LEFT, 0, false);
		leftB.onRelease = std::shared_ptr<ActionWrapper>(new Action<void, GameState&>(std::function<void(GameState&)>(ChangeGameState), gameState));
		renderer->getWindow()->events->mouseBindings.push_back(leftB);

		Binding updateRotate("updateRotate", SDLK_ESCAPE, 0, true);
		updateRotate.onRelease = leftB.onRelease;
		renderer->getWindow()->events->bindings.push_back(updateRotate);
	}

	Raycaster::~Raycaster()
	{
	}

	void	Raycaster::drawMap()
	{
		te::Point2<int> size(mapScale, mapScale);
		te::Point2<int> pos;
		te::Point2<int> startPos(minimapCenter.x  - camera.getPos().x * size.x,
			minimapCenter.y  - camera.getPos().y * size.y);
		te::Point2<double> startPos2(camera.getPos().x - map.size(), camera.getPos().y - map.size());
		pos.y = startPos.y;
		for (size_t y = 0; y < map.size(); y++)
		{
			pos.x = startPos.x;
			for (size_t x = 0; x < map[y].size(); x++)
			{
				if (map[y][x] != 0)
				{
					drawRectangleClamped(pos, size, 0, 0xFF0000FF, renderer, minimapPos, minimapPos + minimapSize);
					drawRectangleClamped(pos, size, 0xFFFFFFFF, renderer, minimapPos, minimapPos + minimapSize);
				}
				else
					drawRectangleClamped(pos, size, 0xFFFFFFFF, renderer, minimapPos, minimapPos + minimapSize);
				pos.x += size.x;
			}
			pos.y += size.y;
		}
		drawRectangle(minimapPos, minimapSize, 0xFFFFFFFF, renderer);
	}

	void	Raycaster::drawPlayerOnMinimap() const
	{
		Point2<int> winPos(minimapCenter.x , minimapCenter.y);
		Point2<int> end(winPos.x + cos(camera.getYaw()) * 70, winPos.y + sin(camera.getYaw()) * 70);
		//drawLineOnMap(winPos, end, 0xFF0000FF, pixels); 
		/*end = te::Point2<int>(winPos.x + cos(topRayAngle) * 100, winPos.y + sin(topRayAngle) * 100);
		drawLineOnMap(winPos, end, 0xFFFFFFFF, pixels);
		end = te::Point2<int>(winPos.x + cos(bottomRayAngle) * 100, winPos.y + sin(bottomRayAngle) * 100);
		drawLineOnMap(winPos, end, 0xFFFFFFFF, pixels);*/
		end = winPos + camera.getRightVec() * 150;
		Point2<int> pos = winPos - camera.getRightVec() * 150;
		drawLineClamped(winPos, end, 0x00FF00FF, renderer, minimapPos, minimapPos + minimapSize);
		drawCircleClamped(winPos, mapScale / 8.0, 0xFFFFFFFF, renderer, minimapPos, minimapPos + minimapSize);
	}

	void Raycaster::render()
	{
		while (running == true)
		{
			if (renderer->getWindow()->handleEvents() == 1)
				running = false;

			drawFloorAndCeiling();

			if (gameState == PLAYING)
			{
				camera.rotateY((renderer->getWindow()->events->mousePos.x - oldMouse.x));
			}
			oldMouse = renderer->getWindow()->events->mousePos;

			drawPlayerOnMinimap();
			drawRays();
			drawMap();

			renderer->render();
		}
	}

	void	Raycaster::drawFloorAndCeiling()
	{
		uint8_t R, G, B;
		double distance;
		std::shared_ptr<SoftwareTexture> ceilingTex = textures[7];
		const unsigned char* img = ceilingTex->getImg();
		for (int y = 0; y < renderer->h / 2; y++)
		{
			distance = (0.5 * renderer->h) / (renderer->h * 0.5 - y);
			double leftX = camera.getPos().x + (camera.getFrontVec().x - camera.getRightVec().x) * distance;
			double rightX = camera.getPos().x + (camera.getFrontVec().x + camera.getRightVec().x) * distance;
			double leftY = camera.getPos().y + (camera.getFrontVec().y - camera.getRightVec().y) * distance;
			double rightY = camera.getPos().y + (camera.getFrontVec().y + camera.getRightVec().y) * distance;
			distance = (0.5 * renderer->h) / (renderer->h * 0.5 - y);
			for (int x = 0; x < renderer->w; x++)
			{
				/*R = 0x25 / distance;
				G = 0x33 / distance;
				B = 0x42 / distance;
				pixels[x + y * (int)renderer->w] = R << 24 | G << 16 | B << 8 | 0xFF;*/
				double currX = leftX + (rightX - leftX) * (x / (double)renderer->w);
				double currY = leftY + (rightY - leftY) * (x / (double)renderer->w);
				//printf("y = %d, x = %d, current pos = [%f %f]\n", y, x, currX, currY);
				//currX = (int)currX % floorTex.getWidth();
				//currY = (int)currY % floorTex.getHeight();
				//printf("y = %d x = %d pos = [%f %f]\n", y, x, currX, currY);
				currX = abs(currX - (int)currX);
				currY = abs(currY - (int)currY);
				//printf("pos = [%f %f]\n", currX, currY);
				currX *= ceilingTex->getWidth();
				currY *= ceilingTex->getHeight();
				int coordX = (int)currX % ceilingTex->getWidth();
				int coordY = (int)currY % ceilingTex->getHeight();
				if (coordX >= ceilingTex->getWidth())
					coordX = ceilingTex->getWidth() - 1;
				if (coordY >= ceilingTex->getHeight())
					coordY = ceilingTex->getHeight() - 1;

				//printf("pos = [%d %d]\n", coordX, coordY);
				int imgCoord = ceilingTex->getChannels() * (int)(coordX + coordY * ceilingTex->getWidth());

				R = img[imgCoord] >> 2;
				G = img[imgCoord + 1] >> 2;
				B = img[imgCoord + 2] >> 2;
				//R = 0x5C / distance;
				//G = 0x62 / distance;
				//B = 0xD6 / distance;
				renderer->pixels[x + y * (int)renderer->w] = R << 24 | G << 16 | B << 8 | 0xFF;
			}
		}
		distance = std::numeric_limits<double>::infinity();
		std::shared_ptr<SoftwareTexture> floorTex = textures[3];
		img = floorTex->getImg();
		//printf("Right dir = [%f %f]\n", camera.getRightVec().x, camera.getRightVec().y);
		for (int y = renderer->h / 2; y < renderer->h; y++)
		{
			distance = (0.5 * renderer->h) / (y - renderer->h * 0.5);
			double leftX = camera.getPos().x + (camera.getFrontVec().x - camera.getRightVec().x) * distance;
			double rightX = camera.getPos().x + (camera.getFrontVec().x + camera.getRightVec().x) * distance;
			double leftY = camera.getPos().y + (camera.getFrontVec().y - camera.getRightVec().y) * distance;
			double rightY = camera.getPos().y + (camera.getFrontVec().y + camera.getRightVec().y) * distance;
			//double currY = camera.getPos().y + camera.getFrontVec().y * distance;
			//printf("y = %d, left x = %f, right x = %f\n", y, leftX, rightX);
			//printf("y = %d, left y = %f, right y = %f\n", y, leftY, rightY);
			//printf("distance = %f\n", distance);
			for (int x = 0; x < renderer->w; x++)
			{
				double currX = leftX + (rightX - leftX) * (x / (double)renderer->w);
				double currY = leftY + (rightY - leftY) * (x / (double)renderer->w);
				//printf("y = %d, x = %d, current pos = [%f %f]\n", y, x, currX, currY);
				//currX = (int)currX % floorTex.getWidth();
				//currY = (int)currY % floorTex.getHeight();
				//printf("y = %d x = %d pos = [%f %f]\n", y, x, currX, currY);
				currX = currX - (int)currX;
				currY = currY - (int)currY;
				if (currX < 0)
					currX = 1 - currX;
				if (currY < 0)
					currY = 1 - currY;
				//printf("pos = [%f %f]\n", currX, currY);
				currX *= floorTex->getWidth();
				currY *= floorTex->getHeight();
				int coordX = (int)currX % floorTex->getWidth();
				int coordY = (int)currY % floorTex->getHeight();
				/*if (coordX < 0)
					coordX = 0;
				if (coordY < 0)
					coordY = 0;*/

				//printf("pos = [%d %d]\n", coordX, coordY);
				int imgCoord = floorTex->getChannels() * (int)(coordX + coordY * floorTex->getWidth());

				R = img[imgCoord] >> 2;
				G = img[imgCoord + 1] >> 2;
				B = img[imgCoord + 2] >> 2;
				//R = 0x5C / distance;
				//G = 0x62 / distance;
				//B = 0xD6 / distance;
				renderer->pixels[x + y * (int)renderer->w] = R << 24 | G << 16 | B << 8 | 0xFF;
			}
			//break;
		}
	}

	void Raycaster::drawRay(Point2<double> pos, double angle, int x, uint32_t color)
	{
		Vector2<double> v(cos(angle), sin(angle));
		double mapX = pos.x;
		double mapY = pos.y;
		double diffX;
		double diffY;
		double newMapX = mapX, newMapY = mapY;
		Point2<double> screen(this->minimapCenter);
		Point2<double> end;
		Point2<int> coord;
		bool hit = false;
		double dist;
		v.normalize();
		//printf("Ray starting from [%f %f] with angle = %f\n", pos.x, pos.y, angle / M_PI * 180);
		//printf("Vector = [%f %f]\n", v.x, v.y);
		int count = 0;
		int textIndex;
		while (hit == false && ((int)mapX < this->mapMaxX && (int)mapY < this->mapMaxY
			&& (int)mapX >= 1 && (int)mapY >= 1))
		{
			//printf("Screen pos = [%d %d]\n", screenX, screenY);
			//if (screenX >= 0 && screenX < renderer->w && screenY >= 0 && screenY < renderer->h)
			//	pixels[screenX + screenY * renderer->w] = color;
			//printf("ceil(%f) = %f\n", 5.0f, ceil(5.0f));
			double nextX;
			if (v.x > 0)
				nextX = floor(mapX) + 1;
			else
				nextX = ceil(mapX) - 1;
			double nextY;
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
			end.x = (screen.x + (newMapX - mapX) * this->mapScale);
			end.y = (screen.y + (newMapY - mapY) * this->mapScale);
			mapX = newMapX;
			mapY = newMapY;

			drawLineClamped(screen, end, color, this->renderer,
				this->minimapPos, this->minimapPos + this->minimapSize);

			coord.x = mapX;
			coord.y = mapY;
			//printf("Coord = [%d %d]\n", coord.x, coord.y);
			if (diffX < diffY && v.x < 0)
				coord.x = (int)(ceil(mapX) - 1);
			if (diffY < diffX && v.y < 0)
				coord.y = (int)(ceil(mapY) - 1);
			//printf("Coord = [%d %d] (%d)\n", coord.x, coord.y, map[coord.y][coord.x]);
			if (this->map[coord.y][coord.x] == 0)
			{
				//drawCircle(end, 4, 0xFFFFFFFF);
			}
			else
			{
				drawCircleClamped(end, this->mapScale / 10.0, 0x00FF00FF, this->renderer,
					this->minimapPos, this->minimapPos + this->minimapSize);
				hit = true;
				//printf("Hit on [%f %f]\n", mapX, mapY);
				//printf("End = [%f %f]\n", end.x, end.y);
				textIndex = this->map[coord.y][coord.x] - 1;
			}
			//drawCircleClamped(end, 4, 0x00FF00FF);
			screen = end;
			count++;
			//if (count == 2)
			//break;
		}
		if (hit == false)
			return ;
		//printf("Intersects at map[%f %f] ([%d %d]\n", mapX, mapY, coord.x, coord.y);
		// TODO Could be replaced by cos/sin formula (cheaper)
		dist = pointDistance(pos, Point2<double>(mapX, mapY));
		//dist = abs(mapX - pos.x) / v.x;
		//double alpha = camera.getAngle() - angle;
		//dist = abs(pos.x - mapX) / cos(camera.getFov());

		//printf("Player angle = %f\n", camera.getAngle());
		//printf("Current angle = %f\n", angle);
		//printf("Dist = %f\n", dist);
		double distCorrec = dist * cos((angle - camera.getYaw()) / (1.0));
		//printf("Correc dist = %f\n", distCorrec);
		double size;
		if (distCorrec < 400 / this->renderer->h)
			size = this->renderer->h / 2;
		else
			size = this->renderer->h / distCorrec;
		double column;
		double dummy;
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
		/*drawLine(Point2<int>(renderer->w - 1 - x, renderer->h / 2 - size),
		Point2<int>(renderer->w - 1 - x, renderer->h / 2 + size),
		color, pixels);*/

		drawColumnOfImg(Point2<int>(x, this->renderer->h / 2 - size / 2), size, column,
			side, this->textures[textIndex]);
	}

	void Raycaster::drawRays()
	{
		double ratio = camera.getHFov() / this->renderer->w;
		double currAngle = camera.getYaw() - camera.getHFov() / 2.0;
		uint32_t color;
		Point2<int> winPos(this->renderer->w / 2, this->renderer->h / 2);

		for (int x = 0; x < this->renderer->w; x++)
		{
			Point2<int> end(winPos.x + cos(currAngle) * 100, winPos.y + sin(currAngle) * 100);
			drawRay(camera.getPos(), currAngle, x, 0xFFFF00FF);
			//printf("Dist[%d] = %f\n", x, dist);
			//printf("angle = %f\n", (camera.getYaw() - currAngle) / (double)M_PI * 180);
			//if (cos(dist) == )
			currAngle += ratio;
			//break;
		}
	}


	void	Raycaster::drawColumnOfImg(Point2<int> start, int length, double column, bool side,
		const std::shared_ptr<SoftwareTexture> texture)
	{
		if (texture->isLoaded() == false)
			return;
		int i = 0;
		const unsigned char* img = texture->getImg();
		int columnIndex = column * texture->getWidth();
		//printf("column index = %d\n", columnIndex);
		while (i < length)
		{
			int rowIndex = (i / (double)length) * texture->getHeight();
			if (start.y + i >= 0 && start.y + i < this->renderer->h
				&& this->renderer->pixels[start.x + (start.y + i) * this->renderer->w] != 0xFFFF00FF
				&& this->renderer->pixels[start.x + (start.y + i) * this->renderer->w] != 0x00FF00FF)
			{
				uint8_t R, G, B;
				int imgCoord =
					texture->getChannels() * (int)(columnIndex + rowIndex * texture->getWidth());

				R = img[imgCoord];
				G = img[imgCoord + 1];
				B = img[imgCoord + 2];
				if (side == false)
				{
					R /= 2;
					G /= 2;
					B /= 2;
				}
				this->renderer->pixels[start.x + (start.y + i) * this->renderer->w] = R << 24 | G << 16 | B << 8 | 0xFF;
			}
			i++;
		}
	}
}