#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <kotono_common/types.h>
class UViewPoint final
{
public:
	UViewPoint();

	const glm::vec3& GetPosition() const;
	const glm::quat& GetRotation() const;
	f32 GetFOV() const;
	f32 GetDepthNear() const;
	
	glm::vec3 GetRightVector() const;
	glm::vec3 GetUpVector() const;
	glm::vec3 GetForwardVector() const;
	
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(const f32 aspectRatio) const;
	
	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::quat& rotation);
	void SetFOV(const f32 yDegFOV);
	void SetDepthNear(const f32 depthNear);

private:
	glm::vec3 position_;
	glm::quat rotation_;

	/// Vertical fov in degrees
	f32 fov_;
	f32 depthNear_;
};
