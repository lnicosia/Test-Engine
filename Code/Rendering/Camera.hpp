#ifndef _CAMERA_HPP_
# define _CAMERA_HPP_

#include "Maths/math_tmp.hpp"

namespace te
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void rotateY(double yaw);
		void rotateX(double roll);
		void rotateZ(double pitch);

		/* Getters */
		const double getYaw() const;
		const double getPitch() const;
		const double getRoll() const;
		const double getHFov() const;
		const double getVFov() const;
		const Point2<double>& getPos() const;
		const Vector2<double>& getFrontVec() const;
		const Vector2<double>& getRightVec() const;

		/* Movement */
		void moveForward();
		void moveBackward();
		void moveLeft();
		void moveRight();

		Point2<double> pos;

	private:
		Vector2<double> front;
		Vector2<double> right;

		double yaw;
		double pitch;
		double roll;

		double vfov;
		double hfov;

	public:
		double moveSpeed;
		double rotationSpeed;
	};

}

#endif