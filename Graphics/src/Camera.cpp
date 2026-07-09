#include "Camera.h"
#include <kotono_platform/glm_utils.h>
#include <kotono_platform/WindowViewport.h>
#include <glm/gtx/string_cast.hpp>

glm::vec3 SCamera::position_{ WorldForwardVector * 3.0f + WorldUpVector };
glm::quat SCamera::rotation_{ glm::angleAxis(glm::radians(180.0f), WorldUpVector) };
f32 SCamera::fov_{ 90.0f };
f32 SCamera::depthNear_{ 0.1f };

const glm::vec3& SCamera::GetPosition()
{
	return position_;
}

const glm::quat& SCamera::GetRotation()
{
	return rotation_;
}

glm::vec3 SCamera::GetRightVector()
{
	return rotation_ * WorldRightVector;
}

glm::vec3 SCamera::GetUpVector()
{
	return rotation_ * WorldUpVector;
}

glm::vec3 SCamera::GetForwardVector()
{
	return rotation_ * WorldForwardVector;
}

glm::mat4 SCamera::GetViewMatrix()
{
	return glm::lookAt(position_, position_ + GetForwardVector(), GetUpVector());
}

glm::mat4 SCamera::GetProjectionMatrix()
{
	return calculate_reverse_z_infinite_perspective(glm::radians(fov_), WindowViewport.GetAspectRatio(), depthNear_);
}

void SCamera::SetPosition(const glm::vec3& position)
{
	position_ = position;
}

void SCamera::SetRotation(const glm::quat& rotation)
{
	rotation_ = rotation;
}

void SCamera::SetFOV(const f32 yDegFOV)
{
	fov_ = yDegFOV;
}

void SCamera::SetDepthNear(const f32 depthNear)
{
	depthNear_ = depthNear;
}
