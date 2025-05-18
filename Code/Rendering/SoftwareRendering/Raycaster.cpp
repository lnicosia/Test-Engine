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
		camera.moveForward(1.0f);
	}

	void Backward(Camera& camera)
	{
		camera.moveBackward(1.0f);
	}

	void Right(Camera& camera)
	{
		camera.moveRight(1.0f);
	}

	void Left(Camera& camera)
	{
		camera.moveLeft(1.0f);
	}

	Raycaster::Raycaster(SoftwareRenderer* renderer): running(true),
		minimapPos(),
		minimapSize(sml::Vector<int, 2>(200, 200)),
		minimapCenter(sml::Vector<int, 2>(minimapPos + minimapSize / 2)),
		mapMaxX(24), mapMaxY(24), mapScale(20.0),
		gameState(PLAYING), renderer(renderer)
	{
		std::string texturesBasePath = Logger::getRootDirPath() + "Resources/Textures/Wolfenstein/";

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

		camera.pos = { 5.0f, 5.0f, 0.0f };

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
		sml::Vector<int, 2> size(static_cast<int>(mapScale), static_cast<int>(mapScale));
		sml::Vector<int, 2> pos;
		sml::Vector<int, 2> startPos(
			minimapCenter.x - static_cast<int>(camera.getPos().x * size.x),
			minimapCenter.y - static_cast<int>(camera.getPos().y * size.y));
		sml::vec2 startPos2(camera.getPos().x - map.size(), camera.getPos().y - map.size());
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
				{
					drawRectangleClamped(pos, size, 0xFFFFFFFF, renderer, minimapPos, minimapPos + minimapSize);
				}
				pos.x += size.x;
			}
			pos.y += size.y;
		}
		drawRectangle(minimapPos, minimapSize, 0xFFFFFFFF, renderer);
	}

	void	Raycaster::drawPlayerOnMinimap() const
	{
		sml::Vector<int, 2> winPos(minimapCenter.x , minimapCenter.y);
		sml::Vector<int, 2> end(
			static_cast<int>(winPos.x + cos(camera.getYaw()) * 70),
			static_cast<int>(winPos.y + sin(camera.getYaw()) * 70));
		//drawLineOnMap(winPos, end, 0xFF0000FF, pixels); 
		/*end = sml::Vector<int, 2>(winPos.x + cos(topRayAngle) * 100, winPos.y + sin(topRayAngle) * 100);
		drawLineOnMap(winPos, end, 0xFFFFFFFF, pixels);
		end = sml::Vector<int, 2>(winPos.x + cos(bottomRayAngle) * 100, winPos.y + sin(bottomRayAngle) * 100);
		drawLineOnMap(winPos, end, 0xFFFFFFFF, pixels);*/
		end = winPos + sml::vec2(camera.getRightVec().x, camera.getRightVec().y) * 150;
		//sml::Vector<int, 2> pos = winPos - camera.getRightVec() * 150;
		drawLineClamped(winPos, end, 0x00FF00FF, renderer, minimapPos, minimapPos + minimapSize);
		drawCircleClamped(winPos, mapScale / 8.0f, 0xFFFFFFFF, renderer, minimapPos, minimapPos + minimapSize);
	}

	void Raycaster::render()
	{
		while (running == true)
		{
			if (renderer->getWindow()->handleEvents() == 1)
			{
				running = false;
			}

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
		float distance;
		std::shared_ptr<SoftwareTexture> ceilingTex = textures[7];
		if (!ceilingTex)
		{
			return;
		}
		const unsigned char* img = ceilingTex->getImg();
		if (!img)
		{
			return ;
		}
		for (int y = 0; y < renderer->h / 2; y++)
		{
			distance = (0.5f * renderer->h) / (renderer->h * 0.5f - y);
			float leftX = camera.getPos().x + (camera.getFrontVec().x - camera.getRightVec().x) * distance;
			float rightX = camera.getPos().x + (camera.getFrontVec().x + camera.getRightVec().x) * distance;
			float leftY = camera.getPos().y + (camera.getFrontVec().y - camera.getRightVec().y) * distance;
			float rightY = camera.getPos().y + (camera.getFrontVec().y + camera.getRightVec().y) * distance;
			distance = (0.5f * renderer->h) / (renderer->h * 0.5f - y);
			for (int x = 0; x < renderer->w; x++)
			{
				/*R = 0x25 / distance;
				G = 0x33 / distance;
				B = 0x42 / distance;
				pixels[x + y * (int)renderer->w] = R << 24 | G << 16 | B << 8 | 0xFF;*/
				float currX = leftX + (rightX - leftX) * (x / (float)renderer->w);
				float currY = leftY + (rightY - leftY) * (x / (float)renderer->w);
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
				{
					coordX = ceilingTex->getWidth() - 1;
				}
				if (coordY >= ceilingTex->getHeight())
				{
					coordY = ceilingTex->getHeight() - 1;
				}

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
		distance = std::numeric_limits<float>::infinity();
		std::shared_ptr<SoftwareTexture> floorTex = textures[3];
		if (!floorTex)
		{
			return;
		}
		img = floorTex->getImg();
		//printf("Right dir = [%f %f]\n", camera.getRightVec().x, camera.getRightVec().y);
		for (int y = renderer->h / 2; y < renderer->h; y++)
		{
			distance = (0.5f * renderer->h) / (y - renderer->h * 0.5f);
			float leftX = camera.getPos().x + (camera.getFrontVec().x - camera.getRightVec().x) * distance;
			float rightX = camera.getPos().x + (camera.getFrontVec().x + camera.getRightVec().x) * distance;
			float leftY = camera.getPos().y + (camera.getFrontVec().y - camera.getRightVec().y) * distance;
			float rightY = camera.getPos().y + (camera.getFrontVec().y + camera.getRightVec().y) * distance;
			//float currY = camera.getPos().y + camera.getFrontVec().y * distance;
			//printf("y = %d, left x = %f, right x = %f\n", y, leftX, rightX);
			//printf("y = %d, left y = %f, right y = %f\n", y, leftY, rightY);
			//printf("distance = %f\n", distance);
			for (int x = 0; x < renderer->w; x++)
			{
				float currX = leftX + (rightX - leftX) * (x / (float)renderer->w);
				float currY = leftY + (rightY - leftY) * (x / (float)renderer->w);
				//printf("y = %d, x = %d, current pos = [%f %f]\n", y, x, currX, currY);
				//currX = (int)currX % floorTex.getWidth();
				//currY = (int)currY % floorTex.getHeight();
				//printf("y = %d x = %d pos = [%f %f]\n", y, x, currX, currY);
				currX = currX - (int)currX;
				currY = currY - (int)currY;
				if (currX < 0)
				{
					currX = 1 - currX;
				}
				if (currY < 0)
				{
					currY = 1 - currY;
				}
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

	void Raycaster::drawRay(sml::vec2 pos, float angle, int x, uint32_t color)
	{
		sml::vec2 v(cos(angle), sin(angle));
		float mapX = pos.x;
		float mapY = pos.y;
		float diffX;
		float diffY;
		float newMapX = mapX, newMapY = mapY;
		sml::vec2 screen(this->minimapCenter);
		sml::vec2 end;
		sml::Vector<int, 2> coord;
		bool hit = false;
		float dist;
		v.normalize();
		//printf("Ray starting from [%f %f] with angle = %f\n", pos.x, pos.y, angle / M_PI * 180);
		//printf("Vector = [%f %f]\n", v.x, v.y);
		//int count = 0;
		int textIndex;
		while (hit == false && ((int)mapX < this->mapMaxX && (int)mapY < this->mapMaxY
			&& (int)mapX >= 1 && (int)mapY >= 1))
		{
			//printf("Screen pos = [%d %d]\n", screenX, screenY);
			//if (screenX >= 0 && screenX < renderer->w && screenY >= 0 && screenY < renderer->h)
			//	pixels[screenX + screenY * renderer->w] = color;
			//printf("ceil(%f) = %f\n", 5.0f, ceil(5.0f));
			float nextX;
			if (v.x > 0)
			{
				nextX = floor(mapX) + 1;
			}
			else
			{
				nextX = ceil(mapX) - 1;
			}
			float nextY;
			if (v.y > 0)
			{
				nextY = floor(mapY) + 1;
			}
			else
			{
				nextY = ceil(mapY) - 1;
			}
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

			coord.x = static_cast<int>(mapX);
			coord.y = static_cast<int>(mapY);
			//printf("Coord = [%d %d]\n", coord.x, coord.y);
			if (diffX < diffY && v.x < 0)
			{
				coord.x = (int)(ceil(mapX) - 1);
			}
			if (diffY < diffX && v.y < 0)
			{
				coord.y = (int)(ceil(mapY) - 1);
			}
			//printf("Coord = [%d %d] (%d)\n", coord.x, coord.y, map[coord.y][coord.x]);
			if (this->map[coord.y][coord.x] == 0)
			{
				//drawCircle(end, 4, 0xFFFFFFFF);
			}
			else
			{
				drawCircleClamped(end, this->mapScale / 10.0f, 0x00FF00FF, this->renderer,
					this->minimapPos, this->minimapPos + this->minimapSize);
				hit = true;
				//printf("Hit on [%f %f]\n", mapX, mapY);
				//printf("End = [%f %f]\n", end.x, end.y);
				textIndex = this->map[coord.y][coord.x] - 1;
			}
			//drawCircleClamped(end, 4, 0x00FF00FF);
			screen = end;
			//count++;
			//if (count == 2)
			//break;
		}
		if (hit == false)
		{
			return ;
		}
		//printf("Intersects at map[%f %f] ([%d %d]\n", mapX, mapY, coord.x, coord.y);
		// TODO Could be replaced by cos/sin formula (cheaper)
		dist = pointDistance(pos, sml::vec2{mapX, mapY});
		//dist = abs(mapX - pos.x) / v.x;
		//float alpha = camera.getAngle() - angle;
		//dist = abs(pos.x - mapX) / cos(camera.getFov());

		//printf("Player angle = %f\n", camera.getAngle());
		//printf("Current angle = %f\n", angle);
		//printf("Dist = %f\n", dist);
		float distCorrec = dist * cosf((angle - camera.getYaw()) / (1.0f));
		//printf("Correc dist = %f\n", distCorrec);
		float size;
		if (distCorrec < 400 / this->renderer->h)
		{
			size = this->renderer->h / 2.0f;
		}
		else
		{
			size = this->renderer->h / distCorrec;
		}
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
		/*drawLine(sml::Vector<int, 2>(renderer->w - 1 - x, renderer->h / 2 - size),
		sml::Vector<int, 2>(renderer->w - 1 - x, renderer->h / 2 + size),
		color, pixels);*/

		drawColumnOfImg(sml::Vector<int, 2>(
			x, static_cast<int>(this->renderer->h / 2.0f - size / 2)),
			static_cast<int>(size), column, side, this->textures[textIndex]);
	}

	void Raycaster::drawRays()
	{
		float ratio = camera.getHFov() / static_cast<float>(this->renderer->w);
		float currAngle = camera.getYaw() - camera.getHFov() / 2.0f;
		//uint32_t color;
		sml::Vector<int, 2> winPos(this->renderer->w / 2, this->renderer->h / 2);

		for (int x = 0; x < this->renderer->w; x++)
		{
			sml::Vector<int, 2> end(
				static_cast<int>(winPos.x + cos(currAngle) * 100),
				static_cast<int>(winPos.y + sin(currAngle) * 100));
			drawRay(sml::vec2(camera.getPos().x, camera.getPos().y), currAngle, x, 0xFFFF00FF);
			//printf("Dist[%d] = %f\n", x, dist);
			//printf("angle = %f\n", (camera.getYaw() - currAngle) / (float)M_PI * 180);
			//if (cos(dist) == )
			currAngle += ratio;
			//break;
		}
	}


	void	Raycaster::drawColumnOfImg(sml::Vector<int, 2> start, int length, float column, bool side,
		const std::shared_ptr<SoftwareTexture> texture)
	{
		if (!texture || texture->isLoaded() == false)
		{
			return;
		}
		int i = 0;
		const unsigned char* img = texture->getImg();
		int columnIndex = static_cast<int>(column * texture->getWidth());
		//printf("column index = %d\n", columnIndex);
		while (i < length)
		{
			int rowIndex = static_cast<int>((i / static_cast<float>(length)) * texture->getHeight());
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