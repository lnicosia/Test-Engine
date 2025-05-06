#include "Camera.hpp"

#include "Debug/Log.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

namespace te
{
	Camera::Camera():
		moveSpeed{0.01f}, rotationSpeed{0.001f},
		pos{2.0f, 2.0f, 2.0f}, view{},
		front{-2.0f, -2.0f, -2.0f},
		right{sml::vec3::fnormalized(sml::vec3::cross(front, up))},
		up{0.0f, 0.0f, 1.0f},
		yaw{0.0f}, pitch{0.0f}, roll{0.0f},
		vfov{90.0/ 180.0 * (float)M_PI}, hfov{90.0/ 180.0 * (float)M_PI}
	{
		view = sml::lookAt(pos, pos + front, up);
	}

	Camera::~Camera()
	{
	}

	void Camera::rotateY(float yaw)
	{
		this->yaw += yaw * rotationSpeed;
		this->front.x = cosf(this->yaw);
		this->front.y = sinf(this->yaw);
		this->right.x = cosf(this->yaw + (float)M_PI / 2.0f);
		this->right.y = sinf(this->yaw +(float) M_PI / 2.0f);
	}

	void Camera::rotateX(float roll)
	{

	}

	void Camera::rotateZ(float pitch)
	{

	}

	const float Camera::getYaw() const
	{
		return yaw;
	}

	const float Camera::getPitch() const
	{
		return pitch;
	}
	
	const float Camera::getRoll() const
	{
		return roll;
	}

	void Camera::moveForward()
	{
		pos += this->front * this->moveSpeed;
		view = sml::lookAt(pos, pos + front, up);
		bIsDirty = true;
	}

	void Camera::moveBackward()
	{
		pos -= this->front * this->moveSpeed;
		view = sml::lookAt(pos, pos + front, up);
		bIsDirty = true;
	}

	void Camera::moveLeft()
	{
		pos -= sml::vec3::fnormalized(sml::vec3::cross(front, up)) * this->moveSpeed;
		view = sml::lookAt(pos, pos + front, up);
		bIsDirty = true;
	}

	void Camera::moveRight()
	{
		pos += sml::vec3::fnormalized(sml::vec3::cross(front, up)) * this->moveSpeed;
		view = sml::lookAt(pos, pos + front, up);
		bIsDirty = true;
	}

	const sml::vec3& Camera::getPos() const
	{
		return pos;
	}

	const sml::vec3& Camera::getFrontVec() const
	{
		return front;
	}

	const sml::vec3& Camera::getRightVec() const
	{
		return right;
	}

	const float Camera::getHFov() const
	{
		return hfov;
	}

	const float Camera::getVFov() const
	{
		return vfov;
	}
}