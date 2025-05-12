#include "Camera.hpp"

#include "Debug/Log.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

namespace te
{
	Camera::Camera():
		moveSpeed{0.01f}, rotationSpeed{0.001f},
		pos{2.0f, 2.0f, 2.0f}, view{},
		front{},
		right{},
		up{0.0f, 0.0f, 1.0f},
		yaw{SML_TO_RADIANS * 225.0f}, pitch{SML_TO_RADIANS * -30.0f}, roll{0.0f},
		vfov{90.0/ 180.0 * (float)M_PI}, hfov{90.0/ 180.0 * (float)M_PI}
	{
		updateView();
	}

	Camera::~Camera()
	{
	}

	void Camera::rotateY(float inYaw)
	{
		yaw += inYaw * rotationSpeed;
		front.x = cosf(yaw);
		front.y = sinf(yaw);
		right.x = cosf(yaw + (float)M_PI / 2.0f);
		right.y = sinf(yaw +(float) M_PI / 2.0f);
	}

	void Camera::rotateX(float roll)
	{

	}

	void Camera::rotateZ(float pitch)
	{

	}

	void Camera::setYaw(float newYaw)
	{
		yaw = newYaw;
		bIsDirty = true;
	}

	void Camera::setPitch(float newPitch)
	{
		pitch = newPitch;
		bIsDirty = true;
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
		pos += front * moveSpeed;
		bIsDirty = true;
	}

	void Camera::moveBackward()
	{
		pos -= front * moveSpeed;
		bIsDirty = true;
	}

	void Camera::moveLeft()
	{
		pos -= right * moveSpeed;
		bIsDirty = true;
	}

	void Camera::moveRight()
	{
		pos += right * moveSpeed;
		bIsDirty = true;
	}

	void Camera::updateView()
	{
		front.x = cosf(yaw) * cosf(pitch);
		front.y = sinf(yaw) * cosf(pitch);
		front.z = sinf(pitch);
		front.fnormalize();
		right = sml::vec3::fnormalized(sml::vec3::cross(front, sml::vec3{0.0f, 0.0f, 1.0f}));
		up = sml::vec3::cross(right, front);
		view = sml::lookAt(pos, pos + front, up);
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