#ifndef _RAYCASTER_HPP_
# define _RAYCASTER_HPP_

#include "Rendering/SoftwareRendering/SoftwareRenderer.hpp"
#include "Rendering/Camera.hpp"
#include "Assets/Textures/SoftwareTexture.hpp"

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
		void	drawRay(Point2<double> pos, double angle, int x, uint32_t color);
		void	drawColumnOfImg(Point2<int> start, int length, double column, bool side,
			const std::shared_ptr<SoftwareTexture> texture);
		void	drawFloorAndCeiling();
		void	drawMap();
		void	drawPlayerOnMinimap() const;

		bool	running;
		std::vector<std::vector<int>>	map;
		Point2<int> minimapPos;
		Point2<int> minimapSize;
		Point2<int> minimapCenter;
		int	mapMaxX;
		int	mapMaxY;
		double	mapScale;
		GameState	gameState;
		Point2<double>	oldMouse;
		
	private:
		SoftwareRenderer* renderer;
		Camera	camera;
		std::vector<std::shared_ptr<SoftwareTexture>> textures;
	};

}

#endif // _RAYCASTER_HPP_