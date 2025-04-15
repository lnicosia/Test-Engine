#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include "Vector.hpp"
#include "MathsException.hpp"

#include <ostream>
#include <iomanip>

namespace sml
{
    template < typename T, size_t rowCount, size_t columnCount >
	struct Matrix
	{
		/** Constructors */
		constexpr Matrix(): data{}
		{
		}

		constexpr Matrix(T value): data{}
		{
			for (size_t i = 0; i < std::min(rowCount, columnCount); i++)
			{
				data[i][i] = value;
			}
			
		}

		constexpr Matrix(std::initializer_list<std::initializer_list<T>> list): data{}
		{
			if (list.size() > rowCount)
			{
				throw MathsException("Too many values in matrix row\n");
			}
			int r = 0;
			for (const std::initializer_list<T>* lit = list.begin(); lit != list.end(); lit++)
			{
				if ((*lit).size() > columnCount)
				{
					throw MathsException("Too many values in matrix row\n");
				}
				int c = 0;
				for (const T* it = (*lit).begin(); it != (*lit).end(); it++)
				{
					data[r][c] = *it;
					c++;
				}
				r++;
			}
		}

		constexpr Matrix(const std::array<std::array<T, rowCount>, columnCount>& from): data(from)
		{
			
		}

		constexpr Matrix(std::initializer_list<Vector<T, columnCount>> list): data{}
		{
			if (list.size() > rowCount)
			{
				throw MathsException("Too many values in matrix row\n");
			}
			int r = 0;
			for (const Vector<T, columnCount>* it = list.begin(); it != list.end(); it++)
			{
				for (size_t i = 0; i < data[r].size(); i++)
				{
					data[r][i] = (*it)[i];
				}
				r++;
			}
		}

		template < typename... Args>
		constexpr Matrix(Args... args): vectors{}
		{
			for (size_t i = 0; i < sizeof...(args); i++)
			{
				// TODO
				//vectors[i] = {args...};
			}
		}

		constexpr void print() const
		{
			for (size_t i = 0; i < rowCount; i++)
			{
				printf("{");
				for (size_t j = 0; j < columnCount; j++)
				{
					printf(" %f", data[i][j]);
				}
				printf(" }\n");
			}
		}

		/** Accessors */
		constexpr std::array<T, columnCount>& operator[]( size_t index )
		{
			if (index >= 0 && index < data.size())
			{
				return data[index];
			}
			throw MathsException("Matrix index out of bounds\n");
		}
		constexpr const std::array<T, columnCount>& operator[]( size_t index ) const
		{
			if (index >= 0 && index < data.size())
			{
				return data[index];
			}
			throw MathsException("Matrix index out of bounds\n");
		}

		/** Operators */
		constexpr Matrix<T, rowCount, columnCount>& operator=( const Matrix<T, rowCount, columnCount>& from ) = default;

		constexpr friend std::ostream& operator<<(std::ostream& o, const Matrix<T, rowCount, columnCount>& m)
		{
			o << std::fixed << std::left;
			o << "{" << std::endl;
			for (size_t i = 0; i < rowCount; i++)
			{
				o << "{";
				for (size_t j = 0; j < columnCount; j++)
				{
					o << " " << std::setw(8) << m[i][j];
				}
				o << " }" << std::endl;
			}
			o << "}" << std::endl;
			return o;
		}

		constexpr explicit operator T*()
		{
			return &(data[0][0]);
		}

		constexpr explicit operator const T*() const
		{
			return &(data[0][0]);
		}

		template < size_t rhsRowCount, size_t rhsColumnCount >
		constexpr Matrix<T, rowCount, rowCount>& operator*=(const Matrix<T, rhsRowCount, rhsColumnCount>& m)
		{
			if (rowCount != rhsColumnCount)
			{
				throw MathsException("Invalid matrix multiplication\n");
			}
			Matrix<T, rowCount, rowCount> res;
			for (size_t i = 0; i < rowCount; i++)
			{
				for (size_t j = 0; j < rowCount; j++)
				{
					for (size_t k = 0; k < columnCount; k++)
					{
						res[i][j] += data[i][k] * m[k][j];
					}
				}
			}
			*this = res;
			return *this;
		}

		template < size_t rhsRowCount, size_t rhsColumnCount >
		constexpr Matrix<T, rowCount, rowCount> operator*(const Matrix<T, rhsRowCount, rhsColumnCount>& m) const
		{
			if (rowCount != rhsColumnCount)
			{
				throw MathsException("Invalid matrix multiplication\n");
			}
			Matrix<T, rowCount, rowCount> res;
			for (size_t i = 0; i < rowCount; i++)
			{
				for (size_t j = 0; j < rowCount; j++)
				{
					for (size_t k = 0; k < columnCount; k++)
					{
						res[i][j] += data[i][k] * m[k][j];
					}
				}
			}
			return res;
		}

		template < typename T, size_t size >
		constexpr Vector<T, size> operator*(const Vector<T, size>& v) const
		{
			Vector<T, size> res{T(0)};
			for (size_t i = 0; i < size; i++)
			{
				for (size_t j = 0; j < size; j++)
				{
					res[i] += data[i][j] * v[j];
				}
			}
			return res;
		}

		/** Data */
		union
		{
			std::array<std::array<T, columnCount>, rowCount> data;
			std::array<Vector<T, columnCount>, rowCount> vectors;
		};
		
	};

	using mat4 = Matrix<float, 4, 4>;
	using mat3 = Matrix<float, 3, 3>;
	using mat2 = Matrix<float, 2, 2>;

	// TODO: make this template deduceable easily
	template < typename T = float, size_t rowCount>
	constexpr Matrix<T, rowCount, rowCount> identity( T value = T(1))
	{
		Matrix<T, rowCount, rowCount> res;
		for (size_t i = 0; i < rowCount; i++)
		{
			res[i][i] = value;
		}
		return res;
	}

	template < typename T >
	constexpr Matrix<T, 4, 4> translate(const Vector<T, 3>& translation)
	{
		return Matrix<T, 4, 4>
		{
			{ 1, 0, 0 , translation.x },
			{ 0, 1, 0 , translation.y },
			{ 0, 0, 1 , translation.z },
			{ 0, 0, 0 , 1 },
		};
	}

	template < typename T >
	constexpr Matrix<T, 4, 4> scale(const Vector<T, 3>& scale)
	{
		return Matrix<T, 4, 4>
		{
			{ scale.x, 0, 0, 0 },
			{ 0, scale.y, 0, 0 },
			{ 0, 0, scale.z, 0 },
			{ 0, 0, 0 , 1 },
		};
	}
	
	template < typename T >
	constexpr Matrix<T, 4, 4> rotate(const T angle, const Vector<T, 3>& axis)
	{
		const float c = std::cos(angle);
		const float s = std::sin(angle);
		const float minc = 1 - c;
		const Vector<T, 3> u = axis.normalized();

		return Matrix<T, 4, 4>{
				{ c + u.x * u.x * minc, u.x * u.y * minc - u.z * s, u.x * u.z * minc + u.y * s, 0 },
				{ u.y * u.x * minc + u.z * s, c + u.y * u.y * minc, u.y * u.z * minc - u.x * s, 0 },
				{ u.z * u.x * minc - u.y * s, u.z * u.y * minc + u.x * s, c + u.z * u.z * minc, 0 },
				{ 0, 0, 0, 1 }};
	}

	template < typename T >
	constexpr Matrix<T, 4, 4> rotateX()
	{
		return Matrix<T, 4, 4>();
	}

	template < typename T >
	constexpr Matrix<T, 4, 4> lookAt(const Vector<T, 3>& pos, const Vector<T, 3>& target, const Vector<T, 3>& up)
	{
		const Vector<T, 3> forward = Vector<T, 3>::fnormalized(target - pos);
		const Vector<T, 3> right = Vector<T, 3>::fnormalized(Vector<T, 3>::cross(forward, up));
		const Vector<T, 3> finalUp = Vector<T, 3>::cross(right, forward);;

		return Matrix<T, 4, 4>{
			{ right.x,		right.y,	right.z,	-Vector<T, 3>::dot(right, pos)},
			{ finalUp.x,	finalUp.y,	finalUp.z,	-Vector<T, 3>::dot(finalUp, pos)},
			{ forward.x,	forward.y,	forward.z,	-Vector<T, 3>::dot(forward, pos)},
			{ 0,			0,			0,			1}};
	}

	template < typename T >
	constexpr Matrix<T, 4, 4> perspective(const T vfov, const T ratio, const T nearPlane, const T farPlane)
	{
		const T tang = std::tan(vfov * T(0.5));
		const T scale = 1 / std::tan(vfov * T(0.5));
		return Matrix<T, 4, 4>{
			{ 1 / (ratio * tang),	0,			0, 0 },
			{ 0,					1 / tang,	0, 0 },
			{ 0,					0,			farPlane / (farPlane - nearPlane), -(farPlane * nearPlane) / (farPlane - nearPlane) },
			{ 0,					0,			1, 0 }
		};
	}

} // namespace sml

#endif // _MATRIX_HPP_