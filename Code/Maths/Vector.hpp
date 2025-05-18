#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include "MathsException.hpp"
#include "MathsUtils.hpp"

#include <array>
#include <iomanip>
#include <cmath>

namespace sml
{
	template < typename T, size_t size >
	struct VectorUnion
	{
		constexpr VectorUnion() = default;

		constexpr VectorUnion(T value)
		{
			for (size_t i = 0; i < size; i++)
			{
				data[i] = value;
			}
		}

		constexpr VectorUnion(const std::array<T, size>& from): data(from)
		{

		}

		template < typename U >
		constexpr VectorUnion(const VectorUnion<U, size>& from): data(from.data)
		{
			
		}

		template < typename ... Tn >
		VectorUnion(Tn... args): data{args...}
		{

		}

		std::array<T, size> data{};

	};

	template < typename T >
	struct VectorUnion<T, 2>
	{
		union
		{
			std::array<T, 2> data;
			struct { T x; T y; };
			struct { T s; T t; };
			struct { T u; T v; };
			struct { T w; T h; };
		};

		constexpr VectorUnion<T, 2>() = default;
		constexpr VectorUnion<T, 2>(T value)
		{
			for (size_t i = 0; i < 2; i++)
			{
				data[i] = value;
			}
		} 
		constexpr VectorUnion<T, 2>( T x, T y): data{x, y} {}
		constexpr VectorUnion<T, 2>(const std::array<T, 2>& from): data(from) {}
	};

	template < typename T >
	struct VectorUnion<T, 3>
	{
		union
		{
			std::array<T, 3> data;
			struct { T x; T y; T z; };
			struct { T s; T t; T p; };
			struct { T r; T g; T b; };
		};

		constexpr VectorUnion<T, 3>() = default;
		constexpr VectorUnion<T, 3>(T value)
		{
			for (size_t i = 0; i < 3; i++)
			{
				data[i] = value;
			}
		} 
		constexpr VectorUnion<T, 3>( T x, T y, T z): data{x, y, z} {}
		constexpr VectorUnion<T, 3>(const std::array<T, 3>& from): data(from) {}
	};

	template < typename T >
	struct VectorUnion<T, 4>
	{
		union
		{
			std::array<T, 4> data;
			struct { T x; T y; T z; T w; };
		};

		constexpr VectorUnion<T, 4>() = default;
		constexpr VectorUnion<T, 4>(T value)
		{
			for (size_t i = 0; i < 4; i++)
			{
				data[i] = value;
			}
		} 
		constexpr VectorUnion<T, 4>( T x, T y, T z, T w): data{x, y, z, w} {}
		constexpr VectorUnion<T, 4>(const std::array<T, 4>& from): data(from) {}
	};

    template < typename T, size_t size >
	struct Vector: public VectorUnion<T, size>
	{
		/** Constructors */
		constexpr Vector(): VectorUnion<T, size>() {}

		constexpr Vector(T value): VectorUnion<T, size>(value) {}

		constexpr Vector(const std::array<T, size>& from): VectorUnion<T, size>(from)
		{

		}

		template < typename U >
		constexpr Vector(const Vector<U, size>& from)
		{
			for (size_t i = 0; i < size; i++)
			{
				(*this)[i] = static_cast<T>(from[i]);
			}
		}

		template < typename ... Tn >
		Vector(Tn... args): VectorUnion<T, size>{args...}
		{

		}

		/** Accessors */
		constexpr T& operator[]( size_t index )
		{
			if (index >= 0 && index < VectorUnion<T, size>::data.size())
			{
				return VectorUnion<T, size>::data[index];
			}
			throw MathsException("Matrix index out of bounds\n");
		}
		constexpr const T& operator[]( size_t index ) const
		{
			if (index >= 0 && index < VectorUnion<T, size>::data.size())
			{
				return VectorUnion<T, size>::data[index];
			}
			throw MathsException("Matrix index out of bounds\n");
		}

		/** Operators */
		constexpr friend std::ostream& operator<<(std::ostream& o, const Vector<T, size>& v)
		{
			o << std::fixed << std::left;
			o << "{" << std::endl;
			for (size_t i = 0; i < size; i++)
			{
				o << " " << std::setw(8) << v[i];
			}
			o << "}" << std::endl;
			return o;
		}

		constexpr Vector<T, size> operator+(const Vector<T, size>& v) const
		{
			Vector<T, size> res;
			for (size_t i = 0; i < size; i++)
			{
				res[i] = (*this)[i] + v[i];
			}
			return res;
		}

		constexpr Vector<T, size> operator-(const Vector<T, size>& v) const
		{
			Vector<T, size> res;
			for (size_t i = 0; i < size; i++)
			{
				res[i] = (*this)[i] - v[i];
			}
			return res;
		}

		constexpr Vector<T, size> operator*(const Vector<T, size>& v) const
		{
			Vector<T, size> res;
			for (size_t i = 0; i < size; i++)
			{
				res[i] = (*this)[i] * v[i];
			}
			return res;
		}

		constexpr Vector<T, size> operator/(const Vector<T, size>& v) const
		{
			Vector<T, size> res;
			for (size_t i = 0; i < size; i++)
			{
				res[i] = (*this)[i] / v[i];
			}
			return res;
		}

		/** Assignment operators */
		constexpr Vector<T, size>& operator+=(const Vector<T, size>& v)
		{
			for (size_t i = 0; i < size; i++)
			{
				(*this)[i] += v[i];
			}
			return *this;
		}

		constexpr Vector<T, size>& operator-=(const Vector<T, size>& v)
		{
			for (size_t i = 0; i < size; i++)
			{
				(*this)[i] -= v[i];
			}
			return *this;
		}

		constexpr Vector<T, size>& operator*=(const Vector<T, size>& v)
		{
			for (size_t i = 0; i < size; i++)
			{
				(*this)[i] ()= v[i];
			}
			return *this;
		}

		constexpr Vector<T, size>& operator/=(const Vector<T, size>& v)
		{
			for (size_t i = 0; i < size; i++)
			{
				(*this)[i] /= v[i];
			}
			return *this;
		}

		constexpr bool operator==(const Vector<T, size>& v) const
		{
			return this->data == v.data;
		}

		constexpr bool operator!=(const Vector<T, size>& v) const
		{
			return this->data != v.data;
		}

		constexpr Vector<T, size> operator*(const T& v) const
		{
			Vector<T, size> res = *this;
			for (size_t i = 0; i < size; i++)
			{
				res[i] *= v;
			}
			return res;
		}

		/** Utilities */
		constexpr void print() const
		{
			printf("{");
			for (size_t i = 0; i < size; i++)
			{
				printf(" %f", (*this)[i]);
			}
			printf(" }");
		}

		constexpr T dot(const Vector<T, size>& other) const
		{
			T res = 0;
			for (size_t i = 0; i < size; i++)
			{
				res += (*this)[i] * other.data[i];
			}
			return res;
		}

		static constexpr T dot(const Vector<T, size>& v1, const Vector<T, size>& v2)
		{
			T res = 0;
			for (size_t i = 0; i < size; i++)
			{
				res += v1.data[i] * v2.data[i];
			}
			return res;
		}

		constexpr Vector<T, 3> cross(const Vector<T, 3>& other) const
		{
			return Vector<T, 3>{
			(VectorUnion<T, size>::data.y * other.z) - (VectorUnion<T, size>::data.z * other.y),
			(VectorUnion<T, size>::data.z * other.x) - (VectorUnion<T, size>::data.x * other.z),
			(VectorUnion<T, size>::data.x * other.y) - (VectorUnion<T, size>::data.y * other.x)};
		}

		static constexpr Vector<T, 3> cross(const Vector<T, size>& v1, const Vector<T, size>& v2)
		{
			return Vector<T, 3>{
			(v1.y * v2.z) - (v1.z * v2.y),
			(v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x)};
		}

		constexpr Vector<T, size> normalize()
		{
			T length = this->length();
			for (size_t i = 0; i < size; i++)
			{
				(*this)[i] /= length;
			}
			return *this;
		}

		/**	Normalized vector = v / |v|
			wich is also v * 1 / |v|
			and |v| is sqrt(v.x * v.x + ... + v.n * v.n)
			which is sqrt(dot(v, v))
			that gives us v * 1 / sqrt(v * v)
			ie v * rsqrt(v * v)*/
		constexpr Vector<T, size> fnormalize()
		{
			*this = *this * sml::frsqrt(Vector<T, size>::dot(*this));
			return *this;
		}

		constexpr Vector<T, size> normalized() const
		{
			Vector<T, size> res = *this;
			T length = this->length();
			for (size_t i = 0; i < size; i++)
			{
				res.data[i] /= length;
			}
			return res;
		}

		/**	Normalized vector = v / |v|
			wich is also v * 1 / |v|
			and |v| is sqrt(v.x * v.x + ... + v.n * v.n)
			which is sqrt(dot(v, v))
			that gives us v * 1 / sqrt(v * v)
			ie v * rsqrt(v * v)*/
		constexpr Vector<T, size> fnormalized() const
		{
			return *this * sml::frsqrt(this->dot(*this));
		}

		static constexpr Vector<T, size> normalized(const Vector<T, size>& v)
		{
			Vector<T, size> res = v;
			T length = v.length();
			for (size_t i = 0; i < size; i++)
			{
				res[i] /= length;
			}
			return res;
		}

		static constexpr Vector<T, size> fnormalized(const Vector<T, size>& v)
		{
			return v * sml::frsqrt(dot(v, v));
		}

		constexpr T length() const
		{
			return std::sqrt(this->dot(*this));
		}

		constexpr T norm() const
		{
			return this->length();
		}

		constexpr T magnitude() const
		{
			return this->length();
		}

		constexpr T min() const
		{
			T m = (*this)[0];
			for (size_t i = 1; i < size; i++)
			{
				if ((*this)[i] < m)
				{
					m = ((*this)[i]);
				}
			}
			return m;
		}

		constexpr T max() const
		{
			T m = (*this)[0];
			for (size_t i = 1; i < size; i++)
			{
				if ((*this)[i] > m)
				{
					m = ((*this)[i]);
				}
			}
			return m;
		}

	};

	using vec2 = Vector<float, 2>;
	using vec3 = Vector<float, 3>;
	using vec4 = Vector<float, 4>;
	
	template < typename T >
	constexpr T pointDistance(const Vector<T, 2>& p1, const Vector<T, 2>& p2)
	{
		T x = p2.x - p1.x;
		T y = p2.y - p1.y;
		return sqrt(x * x + y * y);
	}

	template < typename T, size_t size >
	constexpr Vector<T, size> min(const Vector<T, size>& v1, const Vector<T, size>& v2)
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
		{
			res[i] = std::min(v1[i], v2[i]);
		}
		return res;
	}

	template < typename T, size_t size >
	constexpr Vector<T, size> max(const Vector<T, size>& v1, const Vector<T, size>& v2)
	{
		Vector<T, size> res;
		for (size_t i = 0; i < size; i++)
		{
			res[i] = std::max(v1[i], v2[i]);
		}
		return res;
	}

} // namespace sml

#endif // _VECTOR_HPP_