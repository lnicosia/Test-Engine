#include "DrawUtils.hpp"

namespace te
{
	void drawCircleClamped(const sml::Vector<int, 2> pos, float radius, uint32_t color,
		SoftwareRenderer* renderer, sml::Vector<int, 2> minClamp, sml::Vector<int, 2> maxClamp)
	{
		sml::Vector<int, 2> p;
		float squareRad;

		squareRad = radius * radius;
		for (int y = static_cast<int>(pos.y - radius); y < pos.y + radius; y++)
		{
			for (int x = static_cast<int>(pos.x - radius); x < pos.x + radius; x++)
			{
				p.x = x;
				p.y = y;
				float xDiff = static_cast<float>(pos.x - x);
				float yDiff = static_cast<float>(pos.y - y);
				if (xDiff * xDiff + yDiff * yDiff < squareRad &&
					x >= minClamp.x && x < maxClamp.x
					&& y >= minClamp.y && y < maxClamp.y)
				{
					renderer->pixels[x + y * renderer->w] = color;
				}
			}
		}
	}

	void drawCircle(sml::Vector<int, 2> pos, float radius, uint32_t color, SoftwareRenderer* renderer)
	{
		sml::Vector<int, 2> p;
		float squareRad;

		squareRad = radius * radius;
		for (int y = static_cast<int>(pos.y - radius); y < pos.y + radius; y++)
		{
			for (int x = static_cast<int>(pos.x - radius); x < pos.x + radius; x++)
			{
				p.x = x;
				p.y = y;
				float xDiff = static_cast<float>(pos.x - x);
				float yDiff = static_cast<float>(pos.y - y);
				if (xDiff * xDiff + yDiff * yDiff < squareRad &&
					x >= 0 && x < renderer->w && y >= 0 && y < renderer->h)
				{
					renderer->pixels[x + y * renderer->w] = color;
				}
			}
		}
	}

	void drawCircle(sml::Vector<int, 2> pos, float radius, uint32_t insideColor, uint32_t outsideColor,
		SoftwareRenderer* renderer)
	{

	}

	void drawRectangle(sml::Vector<int, 2> pos, sml::Vector<int, 2> size, uint32_t color, SoftwareRenderer* renderer)
	{
		sml::Vector<int, 2> coord;

		for (int y = 0; y < size.h; y++)
		{
			coord.x = pos.x;
			coord.y = pos.y + y;
			if (coord.x >= 0 && coord.x < renderer->w && coord.y >= 0 && coord.y < renderer->h)
			{
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			}
			coord.x = pos.x + size.w;
			coord.y = pos.y + y;
			if (coord.x >= 0 && coord.x < renderer->w && coord.y >= 0 && coord.y < renderer->h)
			{
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			}
		}
		for (int x = 0; x < size.w && pos.x + x < renderer->w; x++)
		{
			coord.x = pos.x + x;
			coord.y = pos.y;
			if (coord.x >= 0 && coord.x < renderer->w && coord.y >= 0 && coord.y < renderer->h)
			{
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			}
			coord.x = pos.x + x;
			coord.y = pos.y + size.y;
			if (coord.x >= 0 && coord.x < renderer->w && coord.y >= 0 && coord.y < renderer->h)
			{
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			}
		}
	}

	void drawRectangle(sml::Vector<int, 2> pos, sml::Vector<int, 2> size, uint32_t outsideColor, uint32_t insideColor,
		SoftwareRenderer* renderer)
	{
		for (int y = pos.y; y < pos.y + size.h; y++)
		{
			for (int x = pos.x; x < pos.x + size.w; x++)
			{
				if (x >= 0 && x < renderer->w && y >= 0 && y < renderer->h)
				{
					renderer->pixels[x + y * renderer->w] = insideColor;
				}
			}
		}
	}

	void drawRectangleClamped(sml::Vector<int, 2> pos, sml::Vector<int, 2> size, uint32_t color,
		SoftwareRenderer* renderer, sml::Vector<int, 2> minClamp, sml::Vector<int, 2> maxClamp)
	{
		sml::Vector<int, 2> coord;

		for (int y = 0; y < size.h; y++)
		{
			coord.x = pos.x;
			coord.y = pos.y + y;
			if (coord.x >= minClamp.x && coord.x < maxClamp.x
				&& coord.y >= minClamp.y && coord.y < maxClamp.y)
			{
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			}
			coord.x = pos.x + size.w;
			coord.y = pos.y + y;
			if (coord.x >= minClamp.x && coord.x < maxClamp.x
				&& coord.y >= minClamp.y && coord.y < maxClamp.y)
			{
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			}
		}
		for (int x = 0; x < size.w && pos.x + x < renderer->w; x++)
		{
			coord.x = pos.x + x;
			coord.y = pos.y;
			if (coord.x >= minClamp.x && coord.x < maxClamp.x
				&& coord.y >= minClamp.y && coord.y < maxClamp.y)
			{
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			}
			coord.x = pos.x + x;
			coord.y = pos.y + size.y;
			if (coord.x >= minClamp.x && coord.x < maxClamp.x
				&& coord.y >= minClamp.y && coord.y < maxClamp.y)
			{
				renderer->pixels[coord.x + coord.y * renderer->w] = color;
			}
		}
	}

	void drawRectangleClamped(sml::Vector<int, 2> pos, sml::Vector<int, 2> size, uint32_t outsideColor, uint32_t insideColor,
		SoftwareRenderer* renderer, sml::Vector<int, 2> minClamp, sml::Vector<int, 2> maxClamp)
	{
		for (int y = pos.y; y < pos.y + size.h; y++)
		{
			for (int x = pos.x; x < pos.x + size.w; x++)
			{
				if (x >= minClamp.x && x < maxClamp.x
					&& y >= minClamp.y && y < maxClamp.y)
				{
					renderer->pixels[x + y * renderer->w] = insideColor;
				}
			}
		}
	}

	void drawLine(sml::Vector<int, 2> p1, sml::Vector<int, 2> p2, uint32_t color, SoftwareRenderer* renderer)
	{
		sml::vec2 v(p2 - p1);
		float x = static_cast<float>(p1.x);
		float y = static_cast<float>(p1.y);
		v.normalize();
		while ((int)x != p2.x || (int)y != p2.y)
		{
			if (x >= 0 && x < renderer->w && y >= 0 && y < renderer->h)
			{
				renderer->pixels[(int)x + (int)y * renderer->w] = color;
			}
			if ((int)x != p2.x)
			{
				x += v.x;
			}
			if ((int)y != p2.y)
			{
				y += v.y;
			}
		}
	}

	void drawLineClamped(sml::Vector<int, 2> p1, sml::Vector<int, 2> p2, uint32_t color,
		SoftwareRenderer* renderer, sml::Vector<int, 2> minClamp, sml::Vector<int, 2> maxClamp)
	{
		sml::vec2 v(p2 - p1);
		float x = static_cast<float>(p1.x);
		float y = static_cast<float>(p1.y);
		v.normalize();
		while ((int)x != p2.x || (int)y != p2.y)
		{
			if (x >= minClamp.x && x < maxClamp.x
				&& y >= minClamp.y && y < maxClamp.y)
			{
				renderer->pixels[(int)x + (int)y * renderer->w] = color;
			}
			if ((int)x != p2.x)
			{
				x += v.x;
			}
			if ((int)y != p2.y)
			{
				y += v.y;
			}
		}
	}
}