#ifndef _DRAW_UTILS_HPP_
# define _DRAW_UTILS_HPP_

#include "Maths/math_tmp.hpp"
#include "Rendering/SoftwareRendering/SoftwareRenderer.hpp"

namespace te
{
	void drawLine(Point2<int> p1, Point2<int> p2, uint32_t color, SoftwareRenderer* renderer);

	void drawCircle(Point2<int> pos, double radius, uint32_t insideColor, uint32_t outsideColor,
		SoftwareRenderer* renderer);

	void drawCircle(Point2<int> pos, double radius, uint32_t color, SoftwareRenderer* renderer);

	void drawCircleClamped(const Point2<int> pos, double radius, uint32_t color,
		SoftwareRenderer* renderer, Point2<int> minClamp, Point2<int> maxClamp);

	void drawRectangle(Point2<int> pos, Point2<int> size, uint32_t color, SoftwareRenderer* renderer);

	void drawRectangle(Point2<int> pos, Point2<int> size, uint32_t outsideColor, uint32_t insideColor,
		SoftwareRenderer* renderer);

	void drawRectangleClamped(Point2<int> pos, Point2<int> size, uint32_t color,
		SoftwareRenderer* renderer, Point2<int> minClamp, Point2<int> maxClamp);

	void drawRectangleClamped(Point2<int> pos, Point2<int> size, uint32_t outsideColor,
		uint32_t insideColor, SoftwareRenderer* renderer, Point2<int> minClamp, Point2<int> maxClamp);

	void drawLine(Point2<int> p1, Point2<int> p2, uint32_t color, SoftwareRenderer* renderer);

	void drawLineClamped(te::Point2<int> p1, te::Point2<int> p2, uint32_t color,
		SoftwareRenderer* renderer, Point2<int> minClamp, Point2<int> maxClamp);
}

#endif // _DRAW_UTILS_HPP_