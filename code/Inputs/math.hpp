#ifndef _MATH_H_
#define _MATH_H_

template < typename T >
struct Vector2;
template < typename T >
struct Point2;

template < typename T >
struct Point2
{
	union
	{
		struct
		{
			T x;
			T y;
		};
		struct
		{
			T w;
			T h;
		};
	};

	Point2(T x, T y): x(x), y(y)
	{
	}
	template < typename U >
	Point2(U x, U y): x(x), y(y)
	{
	}
	template < typename U >
	Point2(Point2<U> p): x(p.x), y(p.y)
	{
	}
	Point2() : x(), y()
	{
	}

	constexpr Point2<T> operator+(const Vector2<T>& v) const
	{
		return Point2<T>(this->x + v.x, this->y + v.y);
	}

	constexpr Point2<T> operator+(const Point2<T>& v) const
	{
		return Point2<T>(this->x + v.x, this->y + v.y);
	}

	template < typename U >
	constexpr Point2<T> operator+(const Vector2<U>& v) const
	{
		return Point2<T>(this->x + static_cast<T>(v.x), this->y + static_cast<T>(v.y));
	}

	template < typename U >
	constexpr Point2<T> operator+=(const Vector2<U>& v) const
	{
		return Point2<T>(this->x + static_cast<T>(v.x), this->y + static_cast<T>(v.y));
	}

	template < typename U >
	constexpr Point2<T> operator-(const Vector2<U>& v) const
	{
		return Point2<T>(this->x - static_cast<T>(v.x), this->y - static_cast<T>(v.y));
	}

	template < typename U >
	constexpr Point2<T> operator-=(const Vector2<U>& v) const
	{
		return Point2<T>(this->x - static_cast<T>(v.x), this->y - static_cast<T>(v.y));
	}

	constexpr Point2<T> operator*(const T& v) const
	{
		return Point2<T>(this->x * v, this->y * v);
	}

	constexpr Point2<T> operator/(const T& v) const
	{
		return Point2<T>(this->x / v, this->y / v);
	}
};

template < typename T = int >
struct Vector2
{
	union
	{
		struct
		{
			T x;
			T y;
		};
		struct
		{
			T w;
			T h;
		};
	};

	float	norm;

public:
	Vector2(): x(), y(), norm()
	{}

	Vector2(T x, T y): x(x), y(y), norm(sqrt(x * x + y * y))
	{
	}

	Vector2(Point2<T> p1, Point2<T> p2): x(p2.x - p1.x), y(p2.y - p1.y), norm(sqrt(x * x + y * y))
	{
	}

	template < typename U >
	Vector2(Point2<U> p1, Point2<U> p2): x(p2.x - p1.x), y(p2.y - p1.y), norm(sqrt(x * x + y * y))
	{
	}

	void normalize()
	{
		this->x /= norm;
		this->y /= norm;
	}

	constexpr Vector2<T> operator*(const T& v) const
	{
		return Vector2<T>(this->x * v, this->y * v);
	}
};

#endif //_MATH_H_