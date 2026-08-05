#include "ViewPoint.h"
#include <kotono_platform/glm_utils.h>

UViewPoint::UViewPoint()
	: position_{ WorldForwardVector * 3.0f + WorldUpVector }
	, rotation_{ glm::angleAxis(glm::radians(180.0f), WorldUpVector) }
	, fov_{ 90.0f }
	, depthNear_{ 0.1f }
{}

const glm::vec3& UViewPoint::GetPosition() const
{
	return position_;
}

const glm::quat& UViewPoint::GetRotation() const
{
	return rotation_;
}

f32 UViewPoint::GetFOV() const
{
	return fov_;
}

f32 UViewPoint::GetDepthNear() const
{
	return depthNear_;
}

glm::vec3 UViewPoint::GetRightVector() const
{
	return rotation_ * WorldRightVector;
}

glm::vec3 UViewPoint::GetUpVector() const
{
	return rotation_ * WorldUpVector;
}

glm::vec3 UViewPoint::GetForwardVector() const
{
	return rotation_ * WorldForwardVector;
}

glm::mat4 UViewPoint::GetViewMatrix() const
{
	return glm::lookAt(position_, position_ + GetForwardVector(), GetUpVector());
}

glm::mat4 UViewPoint::GetProjectionMatrix(const f32 aspectRatio) const
{
	return calculate_reverse_z_infinite_perspective(glm::radians(fov_), aspectRatio, depthNear_);
}

void UViewPoint::SetPosition(const glm::vec3& position)
{
	position_ = position;
}

void UViewPoint::SetRotation(const glm::quat& rotation)
{
	rotation_ = rotation;
}

void UViewPoint::SetFOV(const f32 yDegFOV)
{
	fov_ = yDegFOV;
}

void UViewPoint::SetDepthNear(const f32 depthNear)
{
	depthNear_ = depthNear;
}
