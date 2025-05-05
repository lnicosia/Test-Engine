#include "math_tmp.hpp"

namespace te
{
	double	pointDistance(const Point2<double>& p1, const Point2<double>& p2)
	{
		double x = p2.x - p1.x;
		double y = p2.y - p1.y;
		return sqrt(x * x + y * y);
	}
}