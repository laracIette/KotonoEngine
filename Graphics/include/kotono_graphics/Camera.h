#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <kotono_common/types.h>
class SCamera final
{
public:
	static const glm::vec3& GetPosition();
	static const glm::quat& GetRotation();
	 
	static glm::vec3 GetRightVector();
	static glm::vec3 GetUpVector();
	static glm::vec3 GetForwardVector();
	 
	static glm::mat4 GetViewMatrix();
	static glm::mat4 GetProjectionMatrix();
	 
	static void SetPosition(const glm::vec3& position);
	static void SetRotation(const glm::quat& rotation);

	static void SetFOV(const f32 yDegFOV);
	static void SetDepthNear(const f32 depthNear);

private:
	static glm::vec3 position_;
	static glm::quat rotation_;

	/// Vertical fov in degrees
	static f32 fov_;
	static f32 depthNear_;
};
