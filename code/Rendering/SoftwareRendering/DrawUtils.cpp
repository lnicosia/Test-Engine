#include "DrawUtils.hpp"

namespace te
{
	void drawCircleClamped(const Point2<int> pos, double radius, uint32_t color,
		SoftwareRenderer* renderer, Point2<int> minClamp, Point2<int> maxClamp)
	{
		Point2<int> p;
		double rad2;

		rad2 = radius * radius;
		for (int y = pos.y - radius; y < pos.y + radius; y++)
		{
			for (int x = pos.x - radius; x < pos.x + radius; x++)
			{
				p.x = x;
				p.y = y;
				double xDiff = pos.x - x;
				double yDiff = pos.y - y;
				if (xDiff * xDiff + yDiff * yDiff < rad2 &&
					x >= minClamp.x && x < maxClamp.x
					&& y >= minClamp.y && y < maxClamp.y)
					renderer->pixels[x + y * renderer->w] = color;
			}
		}
	}

	void drawCircle(Point2<int> pos, double radius, uint32_t color, SoftwareRenderer* renderer)
	{
		Point2<int> p;
		double rad2;

		rad2 = radius * radius;
		for (int y = pos.y - radius; y < pos.y + radius; y++)
		{
			for (int x = pos.x - radius; x < pos.x + radius; x++)
			{
				p.x = x;
				p.y = y;
				double xDiff = pos.x - x;
				double yDiff = pos.y - y;
				if (xDiff * xDiff + yDiff * yDiff < rad2 &&
					x >= 0 && x < renderer->w && y >= 0 && y < renderer->h)
					renderer->pixels[x + y * renderer->w] = color;
			}
		}
	}

	void drawCircle(Point2<int> pos, double radius, uint32_t insideColor, uint32_t outsideColor,
		SoftwareRenderer* renderer)
	{

	}

	void drawRectangle(Point2<int> pos, Point2<int> size, uint32_t color, SoftwareRenderer* renderer)
	{
		Point2<int> coord;

		for (int y = 0; y < size.h; y++)
		{
			coord.x = pos.x;
			coord.y = pos.y + y;
			if (coord.x >= 0 && coord.x < renderer->w && coord.y >= 0 && coord.y < renderer->h)
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			coord.x = pos.x + size.w;
			coord.y = pos.y + y;
			if (coord.x >= 0 && coord.x < renderer->w && coord.y >= 0 && coord.y < renderer->h)
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
		}
		for (int x = 0; x < size.w && pos.x + x < renderer->w; x++)
		{
			coord.x = pos.x + x;
			coord.y = pos.y;
			if (coord.x >= 0 && coord.x < renderer->w && coord.y >= 0 && coord.y < renderer->h)
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			coord.x = pos.x + x;
			coord.y = pos.y + size.y;
			if (coord.x >= 0 && coord.x < renderer->w && coord.y >= 0 && coord.y < renderer->h)
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
		}
	}

	void drawRectangle(Point2<int> pos, Point2<int> size, uint32_t outsideColor, uint32_t insideColor,
		SoftwareRenderer* renderer)
	{
		for (int y = pos.y; y < pos.y + size.h; y++)
		{
			for (int x = pos.x; x < pos.x + size.w; x++)
			{
				if (x >= 0 && x < renderer->w && y >= 0 && y < renderer->h)
					renderer->pixels[x + y * renderer->w] = insideColor;
			}
		}
	}

	void drawRectangleClamped(Point2<int> pos, Point2<int> size, uint32_t color,
		SoftwareRenderer* renderer, Point2<int> minClamp, Point2<int> maxClamp)
	{
		Point2<int> coord;

		for (int y = 0; y < size.h; y++)
		{
			coord.x = pos.x;
			coord.y = pos.y + y;
			if (coord.x >= minClamp.x && coord.x < maxClamp.x
				&& coord.y >= minClamp.y && coord.y < maxClamp.y)
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			coord.x = pos.x + size.w;
			coord.y = pos.y + y;
			if (coord.x >= minClamp.x && coord.x < maxClamp.x
				&& coord.y >= minClamp.y && coord.y < maxClamp.y)
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
		}
		for (int x = 0; x < size.w && pos.x + x < renderer->w; x++)
		{
			coord.x = pos.x + x;
			coord.y = pos.y;
			if (coord.x >= minClamp.x && coord.x < maxClamp.x
				&& coord.y >= minClamp.y && coord.y < maxClamp.y)
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			coord.x = pos.x + x;
			coord.y = pos.y + size.y;
			if (coord.x >= minClamp.x && coord.x < maxClamp.x
				&& coord.y >= minClamp.y && coord.y < maxClamp.y)
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
		}
	}

	void drawRectangleClamped(Point2<int> pos, Point2<int> size, uint32_t outsideColor, uint32_t insideColor,
		SoftwareRenderer* renderer, Point2<int> minClamp, Point2<int> maxClamp)
	{
		for (int y = pos.y; y < pos.y + size.h; y++)
		{
			for (int x = pos.x; x < pos.x + size.w; x++)
			{
				if (x >= minClamp.x && x < maxClamp.x
					&& y >= minClamp.y && y < maxClamp.y)
					renderer->pixels[x + y * renderer->w] = insideColor;
			}
		}
	}

	void	drawLine(Point2<int> p1, Point2<int> p2, uint32_t color, SoftwareRenderer* renderer)
	{
		Vector2<double> v(p1, p2);
		double x = p1.x;
		double y = p1.y;
		v.normalize();
		while ((int)x != p2.x || (int)y != p2.y)
		{
			if (x >= 0 && x < renderer->w && y >= 0 && y < renderer->h)
				renderer->pixels[(int)x + (int)y * renderer->w] = color;
			if ((int)x != p2.x)
				x += v.x;
			if ((int)y != p2.y)
				y += v.y;
		}
	}

	void drawLineClamped(Point2<int> p1, Point2<int> p2, uint32_t color,
		SoftwareRenderer* renderer, Point2<int> minClamp, Point2<int> maxClamp)
	{
		Vector2<double> v(p1, p2);
		double x = p1.x;
		double y = p1.y;
		v.normalize();
		while ((int)x != p2.x || (int)y != p2.y)
		{
			if (x >= minClamp.x && x < maxClamp.x
				&& y >= minClamp.y && y < maxClamp.y)
				renderer->pixels[(int)x + (int)y * renderer->w] = color;
			if ((int)x != p2.x)
				x += v.x;
			if ((int)y != p2.y)
				y += v.y;
		}
	}
}