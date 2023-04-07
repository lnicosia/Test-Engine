#include "Camera.hpp"

namespace te
{
	Camera::Camera():
		pos(0, 0),
		front(1, 0),
		right(front.x + cos(M_PI / 2.0), front.y + sin(M_PI / 2.0)),
		yaw(0.0), pitch(0.0), roll(0.0),
		moveSpeed(0.1), rotationSpeed(0.001),
		hfov(90.0/ 180.0 * (double)M_PI), vfov(90.0/ 180.0 * (double)M_PI)
	{
	}

	Camera::~Camera()
	{
	}

	void	Camera::rotateY(double yaw)
	{
		this->yaw += yaw * rotationSpeed;
		this->front.x = cos(this->yaw);
		this->front.y = sin(this->yaw);
		this->right.x = cos(this->yaw + M_PI / 2.0);
		this->right.y = sin(this->yaw + M_PI / 2.0);
	}

	void	Camera::rotateX(double roll)
	{

	}

	void	Camera::rotateZ(double pitch)
	{

	}

	const double Camera::getYaw() const
	{
		return yaw;
	}
	const double Camera::getPitch() const
	{
		return pitch;
	}
	const double Camera::getRoll() const
	{
		return roll;
	}

	void	Camera::moveForward()
	{
		this->pos.x += this->front.x * this->moveSpeed;
		this->pos.y += this->front.y * this->moveSpeed;
	}

	void	Camera::moveBackward()
	{
		this->pos.x -= this->front.x * this->moveSpeed;
		this->pos.y -= this->front.y * this->moveSpeed;
	}

	void	Camera::moveLeft()
	{
		this->pos.x -= this->right.x * this->moveSpeed;
		this->pos.y -= this->right.y * this->moveSpeed;
	}

	void	Camera::moveRight()
	{
		this->pos.x += this->right.x * this->moveSpeed;
		this->pos.y += this->right.y * this->moveSpeed;
	}

	const Point2<double>& Camera::getPos() const
	{
		return pos;
	}

	const Vector2<double>& Camera::getFrontVec() const
	{
		return front;
	}

	const Vector2<double>& Camera::getRightVec() const
	{
		return right;
	}

	const double Camera::getHFov() const
	{
		return hfov;
	}

	const double Camera::getVFov() const
	{
		return vfov;
	}
}