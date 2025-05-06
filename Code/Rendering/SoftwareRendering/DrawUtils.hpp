#ifndef _DRAW_UTILS_HPP_
# define _DRAW_UTILS_HPP_

#include "Maths/Vector.hpp"
#include "Rendering/SoftwareRendering/SoftwareRenderer.hpp"

namespace te
{
	void drawLine(sml::Vector<int, 2> p1, sml::Vector<int, 2> p2, uint32_t color, SoftwareRenderer* renderer);

	void drawCircle(sml::Vector<int, 2> pos, float radius, uint32_t insideColor, uint32_t outsideColor,
		SoftwareRenderer* renderer);

	void drawCircle(sml::Vector<int, 2> pos, float radius, uint32_t color, SoftwareRenderer* renderer);

	void drawCircleClamped(const sml::Vector<int, 2> pos, float radius, uint32_t color,
		SoftwareRenderer* renderer, sml::Vector<int, 2> minClamp, sml::Vector<int, 2> maxClamp);

	void drawRectangle(sml::Vector<int, 2> pos, sml::Vector<int, 2> size, uint32_t color, SoftwareRenderer* renderer);

	void drawRectangle(sml::Vector<int, 2> pos, sml::Vector<int, 2> size, uint32_t outsideColor, uint32_t insideColor,
		SoftwareRenderer* renderer);

	void drawRectangleClamped(sml::Vector<int, 2> pos, sml::Vector<int, 2> size, uint32_t color,
		SoftwareRenderer* renderer, sml::Vector<int, 2> minClamp, sml::Vector<int, 2> maxClamp);

	void drawRectangleClamped(sml::Vector<int, 2> pos, sml::Vector<int, 2> size, uint32_t outsideColor,
		uint32_t insideColor, SoftwareRenderer* renderer, sml::Vector<int, 2> minClamp, sml::Vector<int, 2> maxClamp);

	void drawLine(sml::Vector<int, 2> p1, sml::Vector<int, 2> p2, uint32_t color, SoftwareRenderer* renderer);

	void drawLineClamped(sml::Vector<int, 2> p1, sml::Vector<int, 2> p2, uint32_t color,
		SoftwareRenderer* renderer, sml::Vector<int, 2> minClamp, sml::Vector<int, 2> maxClamp);
}

#endif // _DRAW_UTILS_HPP_