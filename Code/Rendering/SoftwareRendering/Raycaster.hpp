#ifndef _RAYCASTER_HPP_
# define _RAYCASTER_HPP_

#include "Rendering/SoftwareRendering/SoftwareRenderer.hpp"
#include "Rendering/Camera.hpp"
#include "Assets/Textures/SoftwareTexture.hpp"
#include "Maths/Vector.hpp"

namespace te
{
	enum GameState
	{
		PLAYING,
		PAUSED
	};
	class Raycaster
	{
	public:
		Raycaster() = delete;
		Raycaster(SoftwareRenderer* renderer);
		~Raycaster();

		void	render();
		void	drawRays();
		void	drawRay(sml::vec2 pos, float angle, int x, uint32_t color);
		void	drawColumnOfImg(sml::Vector<int, 2> start, int length, float column, bool side,
			const std::shared_ptr<SoftwareTexture> texture);
		void	drawFloorAndCeiling();
		void	drawMap();
		void	drawPlayerOnMinimap() const;

		bool	running;
		std::vector<std::vector<int>>	map;
		sml::Vector<int, 2> minimapPos;
		sml::Vector<int, 2> minimapSize;
		sml::Vector<int, 2> minimapCenter;
		int	mapMaxX;
		int	mapMaxY;
		float	mapScale;
		GameState	gameState;
		sml::vec2 oldMouse;
		
	private:
		SoftwareRenderer* renderer;
		Camera	camera;
		std::vector<std::shared_ptr<SoftwareTexture>> textures;
	};

}

#endif // _RAYCASTER_HPP_